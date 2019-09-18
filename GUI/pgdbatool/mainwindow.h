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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:

    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionExit_triggered();
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionSQL_Tool_triggered();
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

    void on_bt_txt_clicked();

    void on_bt_grid_clicked();

    void on_bt_html_clicked();

    void on_actionExecute_triggered();

    void on_bt_xml_clicked();

    void on_bt_json_clicked();

private:
    Ui::MainWindow *ui;

    ConnectionsData connections;
    QString path_to_sql;
    QString path_to_models;

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
};

#endif // MAINWINDOW_H
