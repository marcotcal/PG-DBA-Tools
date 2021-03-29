#include "ddlgenerationplugin.h"
#include <dlgparametersschema.h>
#include <dlgparametersequence.h>


DDLGenerationPlugin::DDLGenerationPlugin(QObject *parent) :
    QObject(parent)
{

}

void DDLGenerationPlugin::setMenu(QMenu *menu)
{

}

void DDLGenerationPlugin::setTreeWidget(QTreeWidget *value)
{
    tree = value;
}

void DDLGenerationPlugin::createTree(PGconn *connection)
{
    QTreeWidgetItem *root = new QTreeWidgetItem();
    QTreeWidgetItem *schema_node;
    QTreeWidgetItem *table_node;
    QTreeWidgetItem *view_node;
    QTreeWidgetItem *schema;
    QTreeWidgetItem *table;
    QTreeWidgetItem *view;
    QStringList schemas_list;

    if (connection) {

        schemas_list = schemas(connection);
        root->setText(0, QString(PQdb(connection)));
        root->setIcon(0, QIcon(":/icons/images/icons/icons8-database-48.png"));
        tree->insertTopLevelItem(0, root);


        schema_node = new QTreeWidgetItem();
        schema_node->setText(0, "Schemas");
        schema_node->setIcon(0, QIcon(":/icons/images/icons/icons8-genealogy-40.png"));
        root->addChild(schema_node);

        for(int i=0; i < schemas_list.count(); i++) {

            schema = new QTreeWidgetItem();
            schema->setText(0, schemas_list[i]);
            schema->setIcon(0, QIcon(":/icons/images/icons/icons8-documents-folder-40.png"));
            schema_node->addChild(schema);

            table_node = new QTreeWidgetItem();
            table_node->setText(0, "Tables");
            table_node->setIcon(0, QIcon(":/icons/images/icons/icons8-table-40.png"));
            schema->addChild(table_node);

            view_node = new QTreeWidgetItem();
            view_node->setText(0, "Views");
            schema->addChild(view_node);

        }

    }
}


bool DDLGenerationPlugin::run(PGconn *connection, int item, EditorItem *editor)
{

    /*
    int line, index;
    DlgParametersSchema *dlg_schema;
    DlgParameterSequence *dlg_sequence;

    if (connection) {
        generators = new PGDBAGenerators(connection);
        dlg_schema = new DlgParametersSchema(connection, editor);
        dlg_sequence = new DlgParameterSequence(connection, editor);
    }

    switch(item) {
    case DDL_TEST:
        editor->append("-- Plugin Test.\n");
        editor->append("SELECT 'TESTING DDL GENERATION PLUGIN'\n");
        editor->append("-- End.\n");
        break;
    case DDL_CREATE_SCHEMA:
        if (dlg_schema->exec()) {
            editor->insertAt(dlg_schema->gen_create_schema(), line, index);
        }
        break;
    case DDL_DROP_SCHEMA:
        if (dlg_schema->exec()) {
            editor->insertAt(dlg_schema->gen_drop_schema(), line, index);
        }
        break;
    case DDL_UPDATE_SEQUENCE:
        if (dlg_sequence->exec()) {
            editor->insertAt(dlg_sequence->gen_update_sequece(), line, index);
        }
        break;
    case DDL_RESET_SEQUENCE:
        if (dlg_sequence->exec()) {
            editor->insertAt(dlg_sequence->gen_reset_sequece(), line, index);
        }
        break;
    case DDL_CREATE_TRIGGER:
    case DDL_DROP_TRIGGER:
    case DDL_ENABLE_TRIGGER:
    case DDL_DISABLE_TRIGGER:
    default:
        if (connection) {
            delete generators;
            delete dlg_schema;
            delete dlg_sequence;
        }
        return false;
    }

    if (connection) {
        delete generators;
        delete dlg_schema;
        delete dlg_sequence;
    }
    return true;
    */
}

QStringList DDLGenerationPlugin::schemas(PGconn *connection)
{
    const char *sql =
        "SELECT schema_name "
        "FROM information_schema.schemata "
        "WHERE schema_name NOT IN ('information_schema') AND schema_name !~ '^pg_' ";

    QStringList list;

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

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdba_ddlgenplugin, DDLGenerationPlugin)
#endif // QT_VERSION < 0x050000
