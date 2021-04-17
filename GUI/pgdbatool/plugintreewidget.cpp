#include "plugintreewidget.h"

PluginTreeWidget::PluginTreeWidget(QWidget *parent) : QTreeWidget(parent)
{

}

QListWidget *PluginTreeWidget::getlistWidget() const
{
    return list_widget;
}

void PluginTreeWidget::setListWidget(QListWidget *value)
{
    list_widget = value;
}

PluginElement *PluginTreeWidget::getElement() const
{
    return element;
}

void PluginTreeWidget::setElement(PluginElement *value)
{
    element = value;
}

QTreeWidgetItem *PluginTreeWidget::firstSelected()
{
    QList<QTreeWidgetItem *> selected = selectedItems();
    if (selected.count() > 0)
        return selected.at(0);
    else
        return nullptr;
}
