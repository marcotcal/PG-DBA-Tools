#include "ddlgenerationplugin.h"


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

    item = new QTreeWidgetItem(QStringList() << "Create Schema" << "ITEM_001",1);
    schemas->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Drop Schema" << "ITEM_002",1);
    schemas->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Update Sequences" << "ITEM_003",1);
    sequences->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Reset Sequences" << "ITEM_004",1);
    sequences->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Create Trigger" << "ITEM_005",1);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Drop Trigger" << "ITEM_006",1);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Enable Trigger" << "ITEM_007",1);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Disable Trigger" << "ITEM_008",1);
    triggers->addChild(item);

}

bool DDLGenerationPlugin::run(PGconn *connection, QString item_name, EditorItem *editor)
{
    if (item_name == "TEST_PLUGIN") {

        editor->append("-- Plugin Test.\n");
        editor->append("SELECT 'TESTING PLUGIN'\n");
        editor->append("-- End.\n");

    } else if (item_name == "ITEM_001") {

        editor->append("SELECT 'TESTING ITEM_01'\n");

    }

    return true;

}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdba_ddlgenplugin, DDLGenerationPlugin)
#endif // QT_VERSION < 0x050000
