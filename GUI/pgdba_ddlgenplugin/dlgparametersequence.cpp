#include "dlgparametersequence.h"
#include "ui_dlgparametersequence.h"

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
    const char *params[3];
    /* This query will work only with versions greater than 10
    const char *sql =
            "SELECT 'ALTER SEQUENCE ' || schemaname || '.' || sequencename || E' RESTART;\n' "
            "FROM pg_catalog.pg_sequences ";
    */

    const char *sql =
        "SELECT 'ALTER SEQUENCE ' || n.nspname || '.' || relname || E' RESTART;\n' "
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
        "    AND usename ILIKE $2 || '%' "
        "    AND relname ILIKE $3 || '%' ";

    params[0] = ui->schema_name->currentText().toStdString().c_str();
    params[1] = ui->sequence_owner->currentText().toStdString().c_str();
    params[2] = ui->sequence_name->currentText().toStdString().c_str();

    executeSQL(sql, params, 3);

    for (int i = 0; i < tuples; i++) {
        reset_sequence += (i > 0 ? QString(" ").repeated(index) : "") +
                QString::fromStdString(PQgetvalue(query_result, i, 0));
    }

    clearResult();

    return reset_sequence;

}

QString DlgParameterSequence::gen_update_sequece()
{
    const char *params[3];
    QString update_sequence;
    const char *sql =
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
        "            AND c.relname ILIKE $1 || '%' "
        "            AND d.refobjid::regclass ILIKE $2 || '%' "
        "            AND u.usename ILIKE $3 || '%' "
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

    params[0] = ui->schema_name->currentText().toStdString().c_str();
    params[1] = ui->sequence_owner->currentText().toStdString().c_str();
    params[2] = ui->sequence_name->currentText().toStdString().c_str();

    executeSQL(sql, params, 3);

    for (int i = 0; i < tuples; i++) {
        update_sequence += (i > 0 ? QString(" ").repeated(index) : "") +
                QString::fromStdString(PQgetvalue(query_result, i, 0));
    }

    clearResult();

    return update_sequence;

}

void DlgParameterSequence::on_schema_name_currentIndexChanged(const QString &arg1)
{
    ui->sequence_name->clear();
    ui->sequence_name->addItems(sequences(ui->sequence_owner->currentText(), arg1));
}

void DlgParameterSequence::on_sequence_owner_currentIndexChanged(const QString &arg1)
{
    ui->schema_name->clear();
    ui->schema_name->addItems(schemas(arg1));
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
