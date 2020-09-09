#include "ddlgentreewidget.h"
#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QMessageBox>

DDLGenTreeWidget::DDLGenTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    QTreeWidgetItem* schemas = new QTreeWidgetItem();
    QTreeWidgetItem* sequences = new QTreeWidgetItem();
    QTreeWidgetItem* triggers = new QTreeWidgetItem();
    QTreeWidgetItem* item;

    schemas->setText(0,"Schemas");
    sequences->setText(0,"Sequences");
    triggers->setText(0,"Triggers");

    insertTopLevelItem(0, schemas);
    insertTopLevelItem(1, sequences);
    insertTopLevelItem(2,triggers);

    item = new QTreeWidgetItem(QStringList() << "Create Schema" << ":/ddl/ddl_generators/ddl_create_schemas.xml",1);
    schemas->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Drop Schema" << ":/ddl/ddl_generators/ddl_drop_schemas.xml",1);
    schemas->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Update Sequences" << ":/ddl/ddl_generators/ddl_update_sequences.xml",1);
    sequences->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Reset Sequences" << ":/ddl/ddl_generators/ddl_reset_sequences.xml",1);
    sequences->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Create Trigger" << ":/ddl/ddl_generators/ddl_create_triggers.xml",1);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Drop Trigger" << ":/ddl/ddl_generators/ddl_drop_triggers.xml",1);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Enable Trigger" << ":/ddl/ddl_generators/ddl_enable_triggers.xml",1);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Disable Trigger" << ":/ddl/ddl_generators/ddl_disable_triggers.xml",1);
    triggers->addChild(item);

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(doItemDoubleClicked(QTreeWidgetItem*,int)));

}

void DDLGenTreeWidget::doItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if (item->type() == 1)
        emit executeItem(item->text(1));
}
/*
bool DDLGenTreeWidget::loadPlugins()
{
    QString version;
    PGDBAPluginInterface *interface;
    QDir pluginsDir(QCoreApplication::applicationDirPath());
    bool plugin_loaded = false;

    #if defined(Q_OS_WIN)
        if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
            pluginsDir.cdUp();
    #elif defined(Q_OS_MAC)
        if (pluginsDir.dirName() == "MacOS") {
            pluginsDir.cdUp();
            pluginsDir.cdUp();
            pluginsDir.cdUp();
        }
    #endif

    pluginsDir.cd("plugins");
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entries) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            interface = qobject_cast<PGDBAPluginInterface *>(plugin);
            if (interface) {
                interface_list[interface->plugin_name()] = interface;
                plugin_loaded = true;
            }
        } else {
            QMessageBox::warning(this, "Plugin Error", pluginLoader.errorString());
        }
    }

    return plugin_loaded;
}
*/
