#include "sqlgenerationplugin.h"
#include <dlgparametersschema.h>
#include <dlgparameterobject.h>
#include <dlginsert.h>


SQLGenerationPlugin::SQLGenerationPlugin(QObject *parent) :
    QObject(parent)
{
    list = NULL;
    tree = NULL;
}

void SQLGenerationPlugin::setMenu(QMenu *menu)
{

}

void SQLGenerationPlugin::setTreeWidget(QTreeWidget *value)
{
    tree = value;
}

void SQLGenerationPlugin::setListWidget(QListWidget *value)
{
    list = value;
}

void SQLGenerationPlugin::createTree(PGconn *connection)
{

}

bool SQLGenerationPlugin::run(EditorItem *editor, int item)
{
    int line, index;

    switch(item) {
    case SQL_TEST:
        editor->append("-- Plugin Test.\n");
        editor->append("SELECT 'TESTING SQL GENERATION PLUGIN'\n");
        editor->append("-- End.\n");
        break;
    case SQL_INSERT_ALL:
        editor->getCursorPosition(&line, &index);
        editor->insertAt(gen_insert_all(connection, index), line, index);
        break;
    case SQL_INSERT_MANDATORY:
        editor->getCursorPosition(&line, &index);
        editor->insertAt(gen_insert_mandatory(connection, index), line, index);
        break;

    default:
        return false;
    }

    return true;

}

void SQLGenerationPlugin::updateFunctionList()
{
    if (list)
        list->clear();
}

QStringList SQLGenerationPlugin::schemas(PGconn *connection)
{
    const char *sql =
        "SELECT schema_name "
        "FROM information_schema.schemata "
        "WHERE schema_name NOT IN ('information_schema') AND schema_name !~ '^pg_' ";

    QStringList list;

    list << "";

    int tuples;

    if (PQstatus(connection) == CONNECTION_OK) {
        PGresult *res = PQexec(connection, sql);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
        } else {

            tuples = PQntuples(res);

            for (int i = 0; i < tuples; i++)
                list << QString::fromStdString(PQgetvalue(res, i, 0));

            PQclear(res);

            return list;

        }
    }

    return QStringList();
}

QStringList SQLGenerationPlugin::users(PGconn *connection)
{
    const char *sql =
            "SELECT usename AS role_name, "
            "  CASE "
            "     WHEN usesuper AND usecreatedb THEN "
            "       CAST('superuser, create database' AS pg_catalog.text) "
            "     WHEN usesuper THEN "
            "        CAST('superuser' AS pg_catalog.text) "
            "     WHEN usecreatedb THEN "
            "        CAST('create database' AS pg_catalog.text) "
            "     ELSE "
            "        CAST('' AS pg_catalog.text) "
            "  END role_attributes "
            "FROM pg_catalog.pg_user "
            "ORDER BY role_name desc ";

    QStringList list;

    list << "";

    int tuples;

    if (PQstatus(connection) == CONNECTION_OK) {
        PGresult *res = PQexec(connection, sql);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            PQclear(res);
        } else {

            tuples = PQntuples(res);

            for (int i = 0; i < tuples; i++)
                list << QString::fromStdString(PQgetvalue(res, i, 0));

            PQclear(res);

            return list;

        }
    }

    return QStringList();
}

QString SQLGenerationPlugin::gen_insert_all(PGconn *connection, int offset)
{
    QStringList columns;
        QStringList values;
        QString column_name;
        QString column_default;
        QString data_type;
        QString is_mandatory;
        int tuples;
        QString offset_space = QString(" ").repeated(offset);
        char *error;
        const char *params[2];
        QString insert_stmt;
        const char *sql_list_fields =
            "SELECT column_name, column_default, data_type, is_nullable "
            "FROM information_schema.columns "
            "WHERE "
            "   table_schema = $1 "
            "   AND table_name = $2 ";

        DlgInsert dlg(connection);

        if (dlg.exec()) {

            insert_stmt = QString("INSERT INTO %1.%2 ( \n").arg(dlg.schema()).arg(dlg.table());

            params[0] = dlg.schema().toStdString().c_str();
            params[1] = dlg.table().toStdString().c_str();

            if (PQstatus(connection) == CONNECTION_OK) {

                PGresult *res =  PQexecParams(connection, sql_list_fields, 2, NULL, params, NULL, NULL, 0);

                if (PQresultStatus(res) != PGRES_TUPLES_OK)
                {

                    error = PQerrorMessage(connection);
                    PQclear(res);

                } else {

                    tuples = PQntuples(res);

                    for (int i = 0; i < tuples; i++) {

                        column_name = QString::fromStdString(PQgetvalue(res, i, 0));
                        column_default = QString::fromStdString(PQgetvalue(res, i, 1));
                        data_type = QString::fromStdString(PQgetvalue(res, i, 2));
                        is_mandatory = QString::fromStdString(PQgetvalue(res, i, 3));
                        //if (mandatory && is_mandatory == "NO")
                        //    continue;
                        if (column_default != "")
                            values << offset_space + "   " + column_default + "  /* " + column_name + " */";
                        else if (data_type == "character varying")
                            values << offset_space + "   '' /* " + column_name + " */";
                        else
                            values << offset_space + "      /* " + column_name + " */";
                        columns << offset_space + "   " + column_name;

                    }

                    PQclear(res);
                }
            }

        }

        insert_stmt += columns.join(QByteArray(",\n"));
        insert_stmt += "\n" + offset_space + ") VALUES (\n";
        insert_stmt += values.join(QByteArray(",\n"));
        insert_stmt += "\n" + offset_space + ");";

        return insert_stmt;
}

QString SQLGenerationPlugin::gen_insert_mandatory(PGconn *connection, int offset)
{


}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdba_sqlgenplugin, SQLGenerationPlugin)
#endif // QT_VERSION < 0x050000
