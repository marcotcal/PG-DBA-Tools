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
    element = value;
    QJsonArray keys;

    keys = element->getMeta().toObject().value("Keys").toArray();

    if (keys.contains(QJsonValue("PGDBATOOLS")) && keys.contains(QJsonValue("DDL")) && keys.contains(QJsonValue("DDL_TREE")))
        element->getInterface()->setTreeWidget(this);
}

void DDLGenTreeWidget::doItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if (item->type() != 0)
        emit executeItem(element, item->type());
}

void DDLGenTreeWidget::setConnection(PGconn *value)
{
    connection =value;
}
