#include "sqlgentreewidget.h"
#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QMessageBox>

SQLGenTreeWidget::SQLGenTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(doItemDoubleClicked(QTreeWidgetItem*,int)));
}

void SQLGenTreeWidget::setPluginElement(PluginElement *value)
{
    element = value;
    QJsonArray keys;

    keys = element->getMeta().toObject().value("Keys").toArray();

    if (keys.contains(QJsonValue("PGDBATOOLS")) && keys.contains(QJsonValue("SQL")) && keys.contains(QJsonValue("SQL_TREE")))
        element->getInterface()->setTreeWidget(this);
}

void SQLGenTreeWidget::doItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if (item->type() != 0)
        emit executeItem(element, item->type());
}
