#ifndef DDLGENTREEVIEW_H
#define DDLGENTREEVIEW_H

#include <QTreeWidget>
#include <pgdbaplugininterface.h>
#include <pluginelement.h>

class DDLGenTreeWidget : public QTreeWidget
{    
    Q_OBJECT
public:

    DDLGenTreeWidget(QWidget *parent = Q_NULLPTR);
    void setPluginElement(PluginElement *value);

protected slots:

    void doItemDoubleClicked(QTreeWidgetItem *item, int column);

signals:

    void executeItem(PluginElement *element, int item);

private:

    PluginElement *element;

};

#endif // DDLGENTREEVIEW_H
