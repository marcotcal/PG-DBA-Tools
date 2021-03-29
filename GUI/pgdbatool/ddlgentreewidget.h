#ifndef DDLGENTREEVIEW_H
#define DDLGENTREEVIEW_H

#include <QTreeWidget>
#include <pgdbaplugininterface.h>
#include <pluginelement.h>
#include <libpq-fe.h>


class DDLGenTreeWidget : public QTreeWidget
{    
    Q_OBJECT
public:

    DDLGenTreeWidget(QWidget *parent = Q_NULLPTR);
    void setPluginElement(PluginElement *value);
    void setConnection(PGconn *value);
    void createTree();

protected slots:

    void doItemDoubleClicked(QTreeWidgetItem *item, int column);

signals:

    void executeItem(PluginElement *element, int item);

private:

    QList<PluginElement *>elements;

    PGconn *connection;
};

#endif // DDLGENTREEVIEW_H
