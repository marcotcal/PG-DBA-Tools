#include "dlgparametersequence.h"
#include "ui_dlgparametersequence.h"

/* TODO
 * If a sequence is not created using serial it is not assigned to the table
 * create a function do detect sequences not attached and attatch them using the following code
 *
 * alter sequence my_sequence owned by my_table_manually.id;
 *
 * Now select pg_get_serial_sequence('my_table_manually', 'id') will return a value
 *
 * The queries on this code are not ready yet.
*/


DlgParameterSequence::DlgParameterSequence(PGconn *connection, EditorItem *editor, QWidget *parent) :
    QDialog(parent),
    ParameterBase(connection, editor),
    ui(new Ui::DlgParameterSequence)
{
    ui->setupUi(this);
    ui->sequence_owner->addItems(users());
}

DlgParameterSequence::~DlgParameterSequence()
{
    delete ui;
}

QString DlgParameterSequence::gen_reset_sequece()
{
    QString reset_sequence = "";
    const char *params[4];
    /* This query will work only with versions greater than 10
    const char *sql =
            "SELECT 'ALTER SEQUENCE ' || schemaname || '.' || sequencename || E' RESTART;\n' "
            "FROM pg_catalog.pg_sequences ";
    */

    const char *sql =
        "WITH "
        "    sequences AS ( "
        "        SELECT DISTINCT "
        "            d.refobjid::regclass AS complete_table_name, "
        "            c.relname AS table_name, "
        "            n.nspname AS schema_name, "
        "            a.attname AS field_name, "
        "            pg_get_serial_sequence(d.refobjid::regclass::text, a.attname::text) AS sequence_name, "
        "            u.usename AS owner_name "
        "        FROM "
        "            pg_depend d "
        "            INNER JOIN pg_class c ON c.oid = d.refobjid "
        "            INNER JOIN pg_user u ON c.relowner = u.usesysid "
        "            INNER JOIN pg_namespace n ON c.relnamespace = n.oid "
        "            INNER JOIN pg_attribute a ON a.attrelid = d.refobjid AND a.attnum = d.refobjsubid "
        "        WHERE "
        "            pg_get_serial_sequence(d.refobjid::regclass::text, a.attname::text) IS NOT NULL "
        "    ) "
        "    SELECT 'ALTER SEQUENCE ' || schema_name || '.' || sequence_name || E' RESTART;\n' "
        "    FROM sequences "
        "    WHERE "
        "        schema_name ILIKE $1 || '%' "
        "        AND owner_name ILIKE $2 || '%' "
        "        AND sequence_name ILIKE $3 || '%' "
        "        AND table_name ILIKE $4 || '%' ";

    params[0] = ui->schema_name->currentText().toStdString().c_str();
    params[1] = ui->sequence_owner->currentText().toStdString().c_str();
    params[2] = ui->sequence_name->currentText().toStdString().c_str();
    params[3] = ui->table_name->currentText().toStdString().c_str();

    if (executeSQL(sql, params, 4)) {
        for (int i = 0; i < tuples; i++) {
            reset_sequence += (i > 0 ? QString(" ").repeated(index) : "") +
                    QString::fromStdString(PQgetvalue(query_result, i, 0));
        }

        clearResult();
    } else {
        return QString(PQerrorMessage(connection));
    }


    return reset_sequence;

}

QString DlgParameterSequence::gen_update_sequece()
{
    QString update_sequence;
    QString sql =
        "DO "
        "$$ "
        "DECLARE "
        "    seq record; "
        "    val integer; "
        "BEGIN "
        "    DROP TABLE IF EXISTS sequences; "
        "    CREATE TEMPORARY TABLE sequences ON COMMIT DROP AS "
        "        SELECT DISTINCT "
        "            d.refobjid::regclass AS table_name, "
        "            c.relname, "
        "            n.nspname, "
        "            a.attname AS field_name, "
        "            pg_get_serial_sequence(d.refobjid::regclass::text, a.attname::text) AS sequence_name, "
        "            0 AS value_to_update "
        "        FROM "
        "            pg_depend d "
        "            INNER JOIN pg_class c ON c.oid = d.refobjid "
        "            INNER JOIN pg_user u ON c.relowner = u.usesysid "
        "            INNER JOIN pg_namespace n ON c.relnamespace = n.oid "
        "            INNER JOIN pg_attribute a ON a.attrelid = d.refobjid AND a.attnum = d.refobjsubid "
        "        WHERE "
        "            pg_get_serial_sequence(d.refobjid::regclass::text, a.attname::text) IS NOT NULL "
        "            AND pg_get_serial_sequence(d.refobjid::regclass::text, a.attname::text) ILIKE '%1%' "
        "            AND d.refobjid::regclass::text ILIKE '%2%' "
        "            AND u.usename ILIKE '%3%' "
        "            AND c.relname ILIKE '%4%'; "
        "    FOR seq IN "
        "        SELECT *  FROM sequences "
        "    LOOP "
        "        EXECUTE 'SELECT MAX(' || seq.field_name || ') FROM ' || seq.table_name INTO val; "
        "        UPDATE sequences SET "
        "            value_to_update = val "
        "        WHERE table_name = seq.table_name AND field_name = seq.field_name;  "
        "    END LOOP;  "
        "END; "
        "$$; "
        "SELECT "
        "    CASE "
        "        WHEN value_to_update IS NULL THEN 'ALTER SEQUENCE ' || sequence_name || E' RESTART;\n'  "
        "        ELSE 'ALTER SEQUENCE ' || sequence_name || ' RESTART WITH ' || value_to_update || E';\n'  "
        "    END  "
        "FROM sequences;  ";

    sql = sql.arg(ui->schema_name->currentText()).
            arg(ui->sequence_owner->currentText()).
            arg(ui->sequence_name->currentText()).
            arg(ui->table_name->currentText());

    if(executeSQL(sql.toStdString().c_str(), nullptr, 0)) {

        for (int i = 0; i < tuples; i++) {
            update_sequence += (i > 0 ? QString(" ").repeated(index) : "") +
                    QString::fromStdString(PQgetvalue(query_result, i, 0));
        }

        clearResult();

    } else {
        return QString(PQerrorMessage(connection));
    }

    return update_sequence;

}

void DlgParameterSequence::on_schema_name_currentIndexChanged(const QString &arg1)
{
    ui->sequence_name->clear();
    ui->sequence_name->addItems(sequences(ui->sequence_owner->currentText(), arg1));
    ui->table_name->clear();
    ui->table_name->addItems(tables(ui->sequence_owner->currentText(), arg1));
}

void DlgParameterSequence::on_sequence_owner_currentIndexChanged(const QString &arg1)
{
    ui->schema_name->clear();
    ui->schema_name->addItems(schemas(arg1));
    ui->table_name->clear();
    ui->table_name->addItems(tables(arg1, ui->schema_name->currentText()));
}

QStringList DlgParameterSequence::sequences(QString owner, QString schema_name)
{
    const char *params[2];
    const char *sql =
        "SELECT relname "
        "FROM pg_class c, "
        "     pg_user u, "
        "     pg_namespace n "
        "WHERE "
        "    c.relowner = u.usesysid "
        "    AND c.relkind = 'S' "
        "    AND n.oid = relnamespace "
        "    AND relnamespace IN "
        "        ( "
        "            SELECT oid "
        "                FROM pg_namespace "
        "             WHERE "
        "                nspname NOT LIKE 'pg_%' "
        "                AND nspname != 'information_schema' "
        "                AND nspname ILIKE $1 || '%' "
        "        ) "
        "    AND usename ILIKE $2 || '%' ";
    QStringList list;

    list << "";

    params[0] = schema_name.toStdString().c_str();
    params[1] = owner.toStdString().c_str();

    if(executeSQL(sql, params, 2)) {

        for (int i = 0; i < tuples; i++) {
            list << QString::fromStdString(PQgetvalue(query_result, i, 0));
        }

        clearResult();

    }

    return list;
}

QStringList DlgParameterSequence::tables(QString owner, QString schema_name)
{
    const char *params[2];
    const char *sql =
            "SELECT "
            "    tablename, "
            "    schemaname, "
            "    tablespace "
            "FROM "
            "    pg_catalog.pg_tables "
            "WHERE "
            "    schemaname != 'pg_catalog' "
            "    AND schemaname != 'information_schema' "
            "    AND schemaname ILIKE $1 || '%' "
            "    AND tableowner ILIKE $2 || '%' ";
    QStringList list;

    list << "";

    params[0] = schema_name.toStdString().c_str();
    params[1] = owner.toStdString().c_str();

    if(executeSQL(sql, params, 2)) {

        for (int i = 0; i < tuples; i++) {
            list << QString::fromStdString(PQgetvalue(query_result, i, 0));
        }

        clearResult();

    }

    return list;
}
