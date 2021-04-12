#ifndef PLUGINTREEWIDGET_H
#define PLUGINTREEWIDGET_H

#include <QTreeWidget>
#include <QTabWidget>
#include <pgdbaplugininterface.h>
#include <pluginelement.h>
#include <sqltool.h>
#include <libpq-fe.h>


class PluginTreeWidget : public QTabWidget
{    
    Q_OBJECT
public:

    PluginTreeWidget(QWidget *parent = Q_NULLPTR);
    void setPluginElement(QListWidget *list, PluginElement *value);
    void setConnection(PGconn *value);
    void createTree();
    void clearTrees();
public slots:

    void run_selected_plugin(EditorItem *editor, int item);
    void updateFunctionList(int page);

private:

    QList<PluginElement *>elements;
    QList<QTreeWidget *>element_trees;

    PGconn *connection;
};

#endif // PLUGINTREEWIDGET_H
