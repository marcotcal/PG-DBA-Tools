#include "pgdbagenerators.h"
#include "dlginsert.h"


PGDBAGenerators::PGDBAGenerators()
{
    mandatory = false;
}

QString PGDBAGenerators::getInsert(int offset, PGconn *connection)
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
                    if (mandatory && is_mandatory == "NO")
                        continue;
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

void PGDBAGenerators::setOnlyMandatory(bool value)
{
    mandatory = value;
}
