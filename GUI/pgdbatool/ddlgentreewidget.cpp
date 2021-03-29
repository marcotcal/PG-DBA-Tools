#include "ddlgentreewidget.h"
#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QMessageBox>

DDLGenTreeWidget::DDLGenTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(doItemDoubleClicked(QTreeWidgetItem*,int)));
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setHeaderHidden(true);
}

void DDLGenTreeWidget::setPluginElement(PluginElement *value)
{

    QJsonArray keys;

    keys = value->getMeta().toObject().value("Keys").toArray();

    if (keys.contains(QJsonValue("PGDBATOOLS")) && keys.contains(QJsonValue("DDL")) && keys.contains(QJsonValue("DDL_TREE"))) {
        elements.append(value);
        value->getInterface()->setTreeWidget(this);
    }

}

void DDLGenTreeWidget::doItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    /*
    if (item->type() != 0)
        emit executeItem(element, item->type());
    */
}

void DDLGenTreeWidget::setConnection(PGconn *value)
{
    connection =value;
}

void DDLGenTreeWidget::createTree()
{

    PluginElement *element;

    for(int i = 0; i < elements.count(); i++) {
        element = elements[i];
        element->getInterface()->createTree(connection);
    }

}
