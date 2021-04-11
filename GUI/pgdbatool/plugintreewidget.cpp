#include "plugintreewidget.h"
#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QMessageBox>
#include <QDebug>

PluginTreeWidget::PluginTreeWidget(QWidget *parent) :
    QTabWidget(parent)
{
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(updateFunctionList(int)));
}

void PluginTreeWidget::setPluginElement(QListWidget *list, PluginElement *value)
{

    QJsonArray keys;
    QTreeWidget *tree;

    keys = value->getMeta().toObject().value("Keys").toArray();

    if (keys.contains(QJsonValue("PGDBATOOLS")) &&
            (keys.contains(QJsonValue("DDL")) || keys.contains(QJsonValue("SQL"))) &&
            (keys.contains(QJsonValue("DDL_TREE")) || keys.contains(QJsonValue("SQL_TREE")))) {
        elements.append(value);
        tree = new QTreeWidget(this);
        tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tree->setHeaderHidden(true);

        addTab(tree, value->getMeta().toObject().value("Name").toString());
        value->getInterface()->setTreeWidget(tree);
        value->getInterface()->setListWidget(list);
    }

}

void PluginTreeWidget::setConnection(PGconn *value)
{
    connection =value;
}

void PluginTreeWidget::createTree()
{

    PluginElement *element;

    for(int i = 0; i < elements.count(); i++) {
        element = elements[i];
        element->getInterface()->createTree(connection);
    }

}

void PluginTreeWidget::run_selected_plugin(EditorItem *editor, int item)
{
    elements.at(currentIndex())->getInterface()->run(editor, item);
}

void PluginTreeWidget::updateFunctionList(int page)
{

    elements.at(currentIndex())->getInterface()->updateFunctionList();
}
