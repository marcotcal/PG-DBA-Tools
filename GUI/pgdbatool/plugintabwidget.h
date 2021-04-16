#ifndef PLUGINTABWIDGET_H
#define PLUGINTABWIDGET_H

#include <QTreeWidget>
#include <QTabWidget>
#include <pgdbaplugininterface.h>
#include <pluginelement.h>
#include <sqltool.h>
#include <libpq-fe.h>
#include <plugintreewidget.h>


class PluginTabWidget : public QTabWidget
{    
    Q_OBJECT
public:

    PluginTabWidget(QWidget *parent = Q_NULLPTR);

    void setSqlTool(SqlTool *value);

    QTreeWidget getTreeWidget(PluginElement *element);

    void initializePluginTrees(QListWidget *list);
    void createTree();
    void clearTrees();

public slots:

    void run_selected_plugin(int item);
    void plugin_tab_changed(int index);

private slots:

    void do_update_function_list(QTreeWidgetItem* item, int column);
    void function_activated(QListWidgetItem *item);

private:

    SqlTool *sql_tool;

};

#endif // PLUGINTABWIDGET_H
