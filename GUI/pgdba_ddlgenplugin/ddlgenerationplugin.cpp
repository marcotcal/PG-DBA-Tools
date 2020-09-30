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

void DDLGenerationPlugin::setTreeWidget(QTreeWidget *tree)
{
    QTreeWidgetItem* schemas = new QTreeWidgetItem();
    QTreeWidgetItem* sequences = new QTreeWidgetItem();
    QTreeWidgetItem* triggers = new QTreeWidgetItem();
    QTreeWidgetItem* item;

    schemas->setText(0,"Schemas");
    sequences->setText(0,"Sequences");
    triggers->setText(0,"Triggers");

    tree->insertTopLevelItem(0, schemas);
    tree->insertTopLevelItem(1, sequences);
    tree->insertTopLevelItem(2,triggers);

    item = new QTreeWidgetItem(QStringList() << "Create Schema", DDL_CREATE_SCHEMA);
    schemas->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Drop Schema", DDL_DROP_SCHEMA);
    schemas->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Update Sequences", DDL_UPDATE_SEQUENCE);
    sequences->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Reset Sequences", DDL_RESET_SEQUENCE);
    sequences->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Create Trigger", DDL_CREATE_TRIGGER);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Drop Trigger", DDL_DROP_TRIGGER);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Enable Trigger", DDL_ENABLE_TRIGGER);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Disable Trigger", DDL_DISABLE_TRIGGER);
    triggers->addChild(item);

}

bool DDLGenerationPlugin::run(PGconn *connection, int item, EditorItem *editor)
{
    int line, index;

    DlgParametersSchema dlg_schema(connection, editor);
    DlgParameterSequence dlg_sequence(connection, editor);

    switch(item) {
    case DDL_TEST:
        editor->append("-- Plugin Test.\n");
        editor->append("SELECT 'TESTING PLUGIN'\n");
        editor->append("-- End.\n");
        break;
    case DDL_CREATE_SCHEMA:
        if (dlg_schema.exec()) {
            editor->insertAt(dlg_schema.gen_create_schema(), line, index);
        }
        break;
    case DDL_DROP_SCHEMA:
        if (dlg_schema.exec()) {
            editor->insertAt(dlg_schema.gen_drop_schema(), line, index);
        }
        break;
    case DDL_UPDATE_SEQUENCE:
        if (dlg_sequence.exec()) {
            editor->insertAt(dlg_sequence.gen_update_sequece(), line, index);
        }
        break;
    case DDL_RESET_SEQUENCE:
        if (dlg_sequence.exec()) {
            editor->insertAt(dlg_sequence.gen_reset_sequece(), line, index);
        }
        break;
    case DDL_CREATE_TRIGGER:
    case DDL_DROP_TRIGGER:
    case DDL_ENABLE_TRIGGER:
    case DDL_DISABLE_TRIGGER:
    default:
        return false;
    }

    return true;

}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdba_ddlgenplugin, DDLGenerationPlugin)
#endif // QT_VERSION < 0x050000
