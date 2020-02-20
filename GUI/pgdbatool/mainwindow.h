/***************************************************************************
                      mainwindow.h  -  description
                      ----------------------------
    begin                : Sex Aug 23 2019
    copyright            : (C) 2003 by Marco Túlio Castro
    email                : marco.castro@gmai.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "sqltool.h"
#include "querymodel.h"
#include "connectionsdata.h"
#include "projectdata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:

    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionExit_triggered();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionSQL_Tool_triggered();
    void on_actionSave_SQL_Tool_triggered();
    void on_actionSave_SQL_Tool_As_triggered();
    void on_actionOpen_SQL_Tool_Set_triggered();
    void on_actionManageConnections_triggered();
    void on_actionDatabase_to_Database_triggered();
    void on_actionAdd_Editor_triggered();
    void on_actionClose_Editor_triggered();
    void on_actionCreate_Database_triggered();
    void on_actionDrop_Database_triggered();
    void on_actionStart_Transaction_triggered();
    void on_actionRollback_Transaction_triggered();
    void on_actionCommit_Transaction_triggered();
    void on_actionManage_Plugins_triggered();
    void on_main_stack_currentChanged(int arg1);
    void on_actionConnect_triggered();
    void on_actionDisconect_triggered();
    void on_actionConfigurations_triggered();
    void on_actionClose_triggered();
    void on_editor_list_currentRowChanged(int currentRow);
    void on_actionExecute_triggered();
    void on_actionIndexes_Sizes_triggered();
    void on_connection_list_currentRowChanged(int currentRow);
    void on_actionIndexes_Bloat_triggered();
    void on_actionStat_All_Indexes_triggered();
    void on_actionUseless_Indexes_triggered();
    void on_actionWasted_Index_Space_triggered();
    void on_actionExecute_Saved_Model_triggered();
    void on_actionScan_Model_Directory_triggered();
    void do_fileReaded(const QString &file_name);
    void do_scanCompleted();
    void on_actionDatabases_triggered();
    void on_actionDatabases_Statistics_triggered();
    void on_actionDatabase_Conflicts_triggered();
    void on_actionCache_Hit_Ratio_triggered();
    void on_actionBlocking_Processes_triggered();
    void on_actionBlocking_Process_With_Applications_triggered();
    void on_actionList_of_Tables_triggered();
    void on_actionSchemas_triggered();
    void on_actionTCP_Settings_triggered();
    void on_actionAutovacuum_Settings_triggered();
    void on_actionLog_Settings_triggered();
    void on_actionLanguage_Settings_triggered();
    void on_actionAll_Other_Settings_triggered();
    void on_actionSSL_Settings_triggered();    

    void on_actionExplain_triggered();
    void executeModel(QString resource_name);
    void on_actionPlanner_Method_Settings_triggered();
    void on_actionGenetic_Query_Optimization_Settings_triggered();
    void on_actionPallelism_Settings_triggered();
    void on_actionOther_Planner_Related_Settings_triggered();
    void on_actionWorkers_Settings_triggered();

    void do_beginExecuteQuery(SqlTool *sender);
    void do_endExecuteQuery(SqlTool *sender);
    void on_actionCancel_Current_Query_triggered();
    void on_actionNormal_Functions_triggered();
    void on_actionTrigger_Functions_By_Schema_triggered();
    void on_actionAll_Functions_By_Schema_triggered();
    void on_actionList_of_Views_triggered();

    void on_actionNew_Project_triggered();
    void on_actionProject_Options_triggered();
    void on_actionClose_Project_triggered();
    void on_actionOpen_Project_triggered();

    void projectMenuOpen();

    void on_actionCreate_New_SQL_Script_triggered();

signals:
    void clean_custom_menu();

private:
    Ui::MainWindow *ui;

    ConnectionsData connections;
    ProjectData project;

    QueryModelData *data;
    QString path_to_sql;
    QString path_to_models;
    QString last_path_to_sql;
    int cfg_options;
    QString cfg_task_prefix;

    bool maybeSave();
    void readSettings();
    void writeSettings();
    void disable_actions();
    void enable_sql_tool_actions(SqlTool *sql);
    void enable_model_actions(QueryModel *model);
    void enable_sql_transactions(SqlTool *sql);    
    void setConnectionsList();
    SqlTool *openNewSQLTool(QString name);
    QueryModel *openNewQueryModel(QString name);
    void loadCustomMenus();
};

#endif // MAINWINDOW_H
