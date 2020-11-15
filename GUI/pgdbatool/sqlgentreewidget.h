#ifndef SQLGENTREEWIDGET_H
#define SQLGENTREEWIDGET_H

#include <QTreeWidget>
#include <pgdbaplugininterface.h>
#include <pluginelement.h>

class SQLGenTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:

    SQLGenTreeWidget(QWidget *parent = Q_NULLPTR);
    void setPluginElement(PluginElement *value);

protected slots:

    void doItemDoubleClicked(QTreeWidgetItem *item, int column);

signals:

    void executeItem(PluginElement *element, int item);

private:

    PluginElement *element;

};

#endif // SQLGENTREEWIDGET_H
