#ifndef PLUGINTREEVIEW_H
#define PLUGINTREEVIEW_H

#include <QTreeWidget>
#include <QTabWidget>
#include <pgdbaplugininterface.h>
#include <pluginelement.h>
#include <libpq-fe.h>


class PluginTreeWidget : public QTabWidget
{    
    Q_OBJECT
public:

    PluginTreeWidget(QWidget *parent = Q_NULLPTR);
    void setPluginElement(SqlTool *sql, PluginElement *value);
    void setConnection(PGconn *value);
    void createTree();

protected slots:


signals:


private:

    QList<PluginElement *>elements;
    QList<QTreeWidget *>element_trees;

    PGconn *connection;
};

#endif // PLUGINTREEVIEW_H
