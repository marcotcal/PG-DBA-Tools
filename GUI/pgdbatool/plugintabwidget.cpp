#include "plugintabwidget.h"
#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QMessageBox>
#include <QTreeWidget>
#include <QDebug>
#include "plugintreewidget.h"

PluginTabWidget::PluginTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    sql_tool = nullptr;

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(plugin_tab_changed(int)));
}

void PluginTabWidget::setSqlTool(SqlTool *value)
{
    sql_tool = value;
}

void PluginTabWidget::initializePluginTrees(QListWidget *list)
{
    QMap<QString, PluginElement *> *interface_list = sql_tool->getInterfaceList();
    QJsonArray keys;
    PluginElement *element;
    QString name;
    PluginTreeWidget *tree;

    connect(list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(function_activated(QListWidgetItem*)));

    for(auto i = interface_list->begin(); i != interface_list->end(); i++) {

        element = i.value();

        keys = element->getMeta().toObject().value("Keys").toArray();

        if (keys.contains(QJsonValue("PGDBATOOLS")) &&
                (keys.contains(QJsonValue("DDL")) || keys.contains(QJsonValue("SQL"))) &&
                (keys.contains(QJsonValue("DDL_TREE")) || keys.contains(QJsonValue("SQL_TREE")))) {

            tree = new PluginTreeWidget(this);
            tree->setListWidget(list);
            tree->setElement(element);
            tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
            tree->setHeaderHidden(true);            

            name = element->getMeta().toObject().value("Name").toString();

            addTab(tree, name);

            connect(tree, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(do_update_function_list(QTreeWidgetItem*,int)));

        }
    }
}

void PluginTabWidget::createTree()
{

    PluginElement *element;
    QTreeWidget *tree;
    PGconn *connection;
    QMap<QString, PluginElement *> *interface_list = sql_tool->getInterfaceList();
    int index = 0;

    for(auto i = interface_list->begin(); i != interface_list->end(); i++, index++) {

        element = i.value();
        tree = dynamic_cast<QTreeWidget *>(widget(index));

        if (sql_tool->connected()) {
            connection = sql_tool->getPostgresServiceConnection();
            element->getInterface()->createTree(connection, tree);
        }
    }
}

void PluginTabWidget::clearTrees()
{
    QTreeWidget *tree;

    for(int i = 0; i < count(); i++) {
         tree = dynamic_cast<QTreeWidget *>(widget(i));
         tree->clear();
    }
}

void PluginTabWidget::run_selected_plugin(int item)
{
    PluginTreeWidget *tree = dynamic_cast<PluginTreeWidget *>(currentWidget());
    int line, index;
    QStringList response;

    sql_tool->getCurrentEditor()->getCursorPosition(&line, &index);

    if(tree) {
        response = tree->getElement()->getInterface()->run(
                    tree->firstSelected(), sql_tool->getPostgresConnection(), item);
        foreach(QString str, response) {
            sql_tool->getCurrentEditor()->insertAt(QString(" ").repeated(index) + str, line, 0);
            line++;
        }
    }
}

void PluginTabWidget::plugin_tab_changed(int index)
{
    PluginTreeWidget *tree = dynamic_cast<PluginTreeWidget *>(currentWidget());
    QList<QTreeWidgetItem *> itemList;

    if (tree) {
        itemList = tree->selectedItems();
    }

    foreach(QTreeWidgetItem *item, itemList)
    {
       do_update_function_list(item, 0);
    }
}

void PluginTabWidget::do_update_function_list(QTreeWidgetItem *item, int column)
{
    PluginTreeWidget *tree = dynamic_cast<PluginTreeWidget *>(item->treeWidget());

    if (tree) {
        tree->getElement()->getInterface()->updateFunctionList(item, tree->getlistWidget());
    }
}

void PluginTabWidget::function_activated(QListWidgetItem *item)
{
    int item_type = item->data( Qt::UserRole).toInt();
    run_selected_plugin(item_type);
}
