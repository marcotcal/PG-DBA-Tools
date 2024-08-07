/***************************************************************************
                      mainwindow.cpp  -  description
                      ------------------------------
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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QMenu>
#include <QLibrary>
#include "dlgmenunew.h"
#include "dlgconnections.h"
#include "dlgconfiguration.h"
#include "plaintextoutput.h"
#include "xmltextoutput.h"
#include "jsonoutput.h"
#include "htmloutput.h"
#include "gridoutput.h"
#include "dlgtransaction.h"
#include "querymodeldata.h"
#include "scitextoutput.h"
#include "dlgparameters.h"
#include "dlgexecutemodel.h"
#include "dlgexplain.h"
#include "modelscanner.h"
#include "outputset.h"
#include "dlgproject.h"
#include "frmprojectscript.h"
#include "dlgplugins.h"
#include "plugintabwidget.h"
#include "dlgshowprocesses.h"
#if defined HML_USE_WEBENGINE
    #include <QWebEngineView>
#elif defined HML_USE_WEBKIT
    #include <QWebView>
#else
    #include <qtextbrowser.h>
#endif

#ifdef USE_SSH_TUNNELS
#include "sshconnector.h"
#endif

#include "dlgcreatescript.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readSettings();    
    disable_actions();
    model_data = new QueryModelData(connections, project, this);
    model_data->readModels();
    loadCustomMenus();
    setConnectionsList();
    ui->actionClose->setEnabled(false);

    loadPlugins();

    connect(ui->menuProjects, &QMenu::aboutToShow, this, &MainWindow::projectMenuOpen);
    connect(ui->menuEdit, &QMenu::aboutToShow, this, &MainWindow::editMenuOpen);
    connect(ui->menuView, &QMenu::aboutToShow, this, &MainWindow::viewMenuOpen);

}

bool MainWindow::loadPlugins()
{

    PGDBAPluginInterface *interface;
    QDir pluginsDir(QCoreApplication::applicationDirPath());
    bool plugin_loaded = false;
    QJsonValue meta;
    QJsonArray keys;

    #if defined(Q_OS_WIN)
        if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
            pluginsDir.cdUp();
    #elif defined(Q_OS_MAC)
        if (pluginsDir.dirName() == "MacOS") {
            pluginsDir.cdUp();
            pluginsDir.cdUp();
            pluginsDir.cdUp();
        }
    #endif

    pluginsDir.cd("plugins");
    const QStringList entries = pluginsDir.entryList(QDir::Files);
    for (const QString &fileName : entries) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            meta = pluginLoader.metaData().value("MetaData");
            keys = pluginLoader.metaData().value("MetaData").toObject().value("Keys").toArray();

            interface = qobject_cast<PGDBAPluginInterface *>(plugin);
            if (interface) {
                interface_list[meta.toObject().value("Name").toString()] =
                        new PluginElement(interface, pluginLoader.metaData().value("MetaData"), this);
                plugin_loaded = true;
            }
        } else {
            QMessageBox::warning(this, "Plugin Error", pluginLoader.errorString());
        }
    }

    return plugin_loaded;
}

void MainWindow::projectMenuOpen()
{
    ui->actionClose_Project->setEnabled(!project.getProjectName().isEmpty());
    ui->actionProject_Options->setEnabled(!project.getProjectName().isEmpty());
    ui->actionCreate_New_SQL_Script->setEnabled(!project.getProjectName().isEmpty());
    ui->actionExecute_Project_Script->setEnabled(!project.getProjectName().isEmpty());
}

void MainWindow::editMenuOpen()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());

    if (sql) {
        // edit menu
        ui->actionFind->setEnabled(sql->isFindAvailable());
        ui->actionFind_Next->setEnabled(sql->isFindNextAvailable());
        ui->actionCut->setEnabled(sql->isCutAvailable());
        ui->actionCopy->setEnabled(sql->isCopyAvailable());
        ui->actionPaste->setEnabled(sql->isPasteAvailabe());
        ui->actionUndo->setEnabled(sql->isUndoAvailable());
        ui->actionRedo->setEnabled(sql->isRedoAvailable());
        ui->actionDelete->setEnabled(sql->isDeleteAvailable());
        ui->actionSelect_All->setEnabled(sql->isSelectAllAvailable());
        // end of edit menu
    } else {
        ui->actionFind->setEnabled(false);
        ui->actionFind_Next->setEnabled(false);
        ui->actionCut->setEnabled(false);
        ui->actionCopy->setEnabled(false);
        ui->actionPaste->setEnabled(false);
        ui->actionUndo->setEnabled(false);
        ui->actionRedo->setEnabled(false);
        ui->actionDelete->setEnabled(false);
        ui->actionSelect_All->setEnabled(false);
    }
}

void MainWindow::viewMenuOpen()
{
    ui->actionViewMaintenance_Connections->setChecked(ui->dock_connections->isVisible());
    ui->actionViewMessages->setChecked(ui->dock_messages->isVisible());
    ui->actionViewOutputs->setChecked(ui->dock_output->isVisible());
    ui->actionViewSql_Editors_List->setChecked(ui->doc_sql_editors->isVisible());
    ui->actionViewCode_Generators->setChecked(ui->dock_plugin->isVisible());
    ui->actionViewCode_Function->setChecked(ui->dock_function->isVisible());
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::maybeSave()
{
    SqlTool *sql;
    QMessageBox msgbox;

    for (int i = 0; i < ui->main_stack->count(); i++) {
        ui->main_stack->setCurrentIndex(i);
        sql = dynamic_cast<SqlTool*>(ui->main_stack->widget(i));
        if (sql) {
            ui->editor_list->findItems(sql->getGroupName(),Qt::MatchExactly)
                    .at(0)->setSelected(true);
            if (sql->isModified()) {
                ui->main_stack->setCurrentIndex(i);
                msgbox.setText("The SQL Tool has been modified.");
                msgbox.setInformativeText("Do you want to save your changes?");
                msgbox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
                msgbox.setDefaultButton(QMessageBox::Save);
                switch(msgbox.exec()) {
                case QMessageBox::Save:
                    return sql->saveGroup();
                case QMessageBox::Discard:
                    return true;
                case QMessageBox::Cancel:
                    return false;
                default:
                    break;
                }
            }
        }
    }

    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    path_to_sql = settings.value("path_to_sql", "").toString();
    last_path_to_sql = path_to_sql;
    path_to_models = settings.value("path_to_models", "").toString();
    cfg_options = settings.value("config_options", (1+2+4)).toInt();
    cfg_task_prefix = settings.value("config_task_prefix", "").toString();

    project.setProjectPath(settings.value("project_path", "").toString());
    project.readConfig();

    if (project.getProjectName() != "") {

        setWindowTitle("PostgreSQL DBA Tool - " + project.getProjectName());
        project.loadConnections();

    } else {

        setWindowTitle("PostgreSQL DBA Tool");

    }

    restoreState(settings.value("DOCK_LOCATIONS").toByteArray(),1);
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }

    if (!project.getProjectPath().isEmpty()) {
        connections.readConnections(project.getProjectPath() + "/config");
        path_to_sql = project.getQueryPath();
        last_path_to_sql = path_to_sql;
        path_to_models = project.getModelPath();
    } else
        connections.readConnections();
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("path_to_models", path_to_models);
    settings.setValue("path_to_sql", path_to_sql);
    settings.setValue("DOCK_LOCATIONS",this->saveState(1));
    settings.setValue("project_path", project.getProjectPath());
    settings.setValue("config_options", cfg_options);
    settings.setValue("config_task_prefix", cfg_task_prefix);
    if (project.getProjectName() != "")
        connections.writeConnections(project.getProjectPath()+"/config");
    else
        connections.writeConnections();
}

void MainWindow::disable_actions()
{
    ui->actionSave->setEnabled(false);
    ui->actionSave_As->setEnabled(false);
    ui->actionSave_SQL_File->setEnabled(false);
    ui->actionSave_SQL_File_As->setEnabled(false);
    ui->actionOpen_SQL_File->setEnabled(false);
    ui->actionAdd_Editor->setEnabled(false);
    ui->actionClose_Editor->setEnabled(false);
    ui->actionStart_Transaction->setEnabled(false);
    ui->actionCommit_Transaction->setEnabled(false);
    ui->actionRollback_Transaction->setEnabled(false);
    ui->actionExecute->setEnabled(false);
    ui->actionExplain->setEnabled(false);
    ui->actionConnect->setEnabled(false);
    ui->actionDisconect->setEnabled(false);
    ui->actionCancel_Current_Query->setEnabled(false);
    ui->actionPlanner_Method_Settings->setEnabled(false);
    ui->actionGenetic_Query_Optimization_Settings->setEnabled(false);
    ui->actionPallelism_Settings->setEnabled(false);
    ui->actionOther_Planner_Related_Settings->setEnabled(false);
    ui->actionWorkers_Settings->setEnabled(false);    
}

void MainWindow::enable_sql_tool_actions(SqlTool *sql)
{    

    if (sql->getMode() == SqlTool::MODE_QUERY || sql->getMode() == SqlTool::MODE_SCRIPT) {
        ui->actionAdd_Editor->setEnabled(true);
        ui->actionClose_Editor->setEnabled(true);
    }
    ui->actionSave->setEnabled(true);
    ui->actionSave_As->setEnabled(true);
    ui->actionSave_SQL_File->setEnabled(true);
    ui->actionSave_SQL_File_As->setEnabled(true);
    ui->actionOpen_SQL_File->setEnabled(true);

    ui->actionConnect->setEnabled(!sql->connected() && !sql->isRunning());
    ui->actionDisconect->setEnabled(sql->connected() && !sql->isRunning());
    // implicity transactions
    ui->actionExecute->setEnabled(sql->connected()  && !sql->isRunning());
    ui->actionExplain->setEnabled(sql->connected()  && !sql->isRunning());
    ui->actionPlanner_Method_Settings->setEnabled(sql->connected()  && !sql->isRunning());
    ui->actionGenetic_Query_Optimization_Settings->setEnabled(sql->connected()  && !sql->isRunning());
    ui->actionPallelism_Settings->setEnabled(sql->connected()  && !sql->isRunning());
    ui->actionOther_Planner_Related_Settings->setEnabled(sql->connected()  && !sql->isRunning());
    ui->actionWorkers_Settings->setEnabled(sql->connected()  && !sql->isRunning());

    ui->actionCancel_Current_Query->setEnabled(sql->isRunning());

    if (sql->connected()) {
        if (sql->getMode() == SqlTool::MODE_QUERY) {
            ui->actionStart_Transaction->setEnabled(!sql->isRunning());
        } else {
            ui->actionStart_Transaction->setEnabled(false);
        }
        ui->actionCommit_Transaction->setEnabled(false);
        ui->actionRollback_Transaction->setEnabled(false);     
    } else {
        ui->actionStart_Transaction->setEnabled(false);
        ui->actionCommit_Transaction->setEnabled(false);
        ui->actionRollback_Transaction->setEnabled(false);
    }

}

void MainWindow::enable_model_actions(QueryModel *model)
{
    Q_UNUSED(model)
    ui->actionSave->setEnabled(true);
    ui->actionSave_As->setEnabled(true);

}

void MainWindow::enable_sql_transactions(SqlTool *sql) {
    if (!sql->connected())
        return;

    if (sql->getMode() == SqlTool::MODE_QUERY) {
        ui->actionStart_Transaction->setEnabled(!sql->transaction() && !sql->isRunning());
        ui->actionCommit_Transaction->setEnabled(sql->transaction() && !sql->isRunning());
        ui->actionRollback_Transaction->setEnabled(sql->transaction() && !sql->isRunning());
    }

    ui->actionExecute->setEnabled(sql->connected() && !sql->isRunning());
    ui->actionExplain->setEnabled(sql->connected() && !sql->isRunning());
    ui->actionCancel_Current_Query->setEnabled(sql->connected() && !sql->isRunning());
    ui->actionPlanner_Method_Settings->setEnabled(sql->connected() && !sql->isRunning());
    ui->actionGenetic_Query_Optimization_Settings->setEnabled(sql->connected() && !sql->isRunning());
    ui->actionPallelism_Settings->setEnabled(sql->connected() && !sql->isRunning());
    ui->actionOther_Planner_Related_Settings->setEnabled(sql->connected() && !sql->isRunning());
    ui->actionWorkers_Settings->setEnabled(sql->connected() && !sql->isRunning());

    ui->actionSave->setEnabled(true);
    ui->actionSave_As->setEnabled(true);
    ui->actionSave_SQL_File->setEnabled(true);
    ui->actionSave_SQL_File_As->setEnabled(true);
    ui->actionOpen_SQL_File->setEnabled(true);
}

void MainWindow::setConnectionsList()
{
    QListWidgetItem *item;
    connections.sortByName();
    ui->connection_list->clear();
    for (int i = 0; i < connections.getConnections().count(); i++) {
        ConnectionElement *conn = connections.getConnections().at(i);
        if (conn->isInvalid()) {
            item = new QListWidgetItem(conn->name() + " (Invallid)", ui->connection_list);
            item->setForeground(Qt::red);
        } else {
             item = new QListWidgetItem(conn->name(), ui->connection_list);
        }
    }
    if (ui->connection_list->count() > 0)
        ui->connection_list->setCurrentRow(0);
}

SqlTool *MainWindow::openNewSQLTool(QString name, int mode)
{
    SqlTool *sql;
    OutputSet *out;
    QListWidgetItem *list_item;

    list_item = new QListWidgetItem(name, ui->editor_list);

    switch(mode) {
    case SqlTool::MODE_INTERNAL_DEVELOPMENT:
    case SqlTool::MODE_INTERNAL_STAGING:
    case SqlTool::MODE_INTERNAL_PRODUCTION:
    case SqlTool::MODE_QUERY:
    case SqlTool::MODE_SCRIPT:
        sql = new SqlTool(connections, ui->connection_list->currentRow(), project, ui->main_stack);
        sql->addEditor();
        break;
    default:
        return nullptr;
    }

    sql->setInterfaceList(&interface_list);

    sql->setMode(mode);

    ui->main_stack->addWidget(sql);
    ui->main_stack->setCurrentWidget(ui->main_stack);
    ui->editor_list->clearSelection();
    sql->setGroupName(name);

    out = new OutputSet(sql);
    sql->setOutputSet(out);
    ui->output_stack->addWidget(out);
    ui->output_stack->setCurrentWidget(out);

    ui->plugin_stack->addWidget(sql->getPluginWidgetTree());
    ui->plugin_stack->setCurrentWidget(sql->getPluginWidgetTree());

    ui->function_stack->addWidget(sql->getFunctionList());
    ui->function_stack->setCurrentWidget(sql->getFunctionList());

    //connect(ddl_tree, SIGNAL(executeItem(PluginElement*,int)), this, SLOT(executePlugin(PluginElement*,int)));

    //for(auto i = interface_list.begin(); i != interface_list.end(); i++)
    //    ddl_tree->setPluginElement(function_list, i.value());

    ui->main_stack->setCurrentWidget(sql);
    ui->editor_list->setCurrentRow(ui->main_stack->currentIndex());
    connect(sql, SIGNAL(beginExecution(SqlTool *)), this, SLOT(do_beginExecuteQuery(SqlTool*)));
    connect(sql, SIGNAL(endExecution(SqlTool *)), this, SLOT(do_endExecuteQuery(SqlTool*)));
    connect(sql, SIGNAL(modeChanged(SqlTool *, int)), this, SLOT(sqlToolModeChanged(SqlTool *, int)));
    connect(sql, SIGNAL(requestToClose()), this, SLOT(closeRequested()));
    return sql;

}

QueryModel *MainWindow::openNewQueryModel(QString name)
{
    QueryModel *qmod;

    new QListWidgetItem(name, ui->editor_list);
    qmod = new QueryModel(connections, project, ui->main_stack);
    ui->main_stack->addWidget(qmod);
    ui->main_stack->setCurrentWidget(qmod);
    ui->editor_list->clearSelection();
    ui->editor_list->setCurrentRow(ui->main_stack->currentIndex());

    return qmod;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionNew_triggered()
{
    DlgMenuNew *dlg = new DlgMenuNew(this);
    QString name;

    if (dlg->exec() == QDialog::Accepted) {

        switch(dlg->getSelection()) {
        case 0:
            name = QString("Sql Tool %1").arg(ui->main_stack->count()+1);
            openNewSQLTool(name);
            break;
        case 1:
            name = QString("Query Model %1").arg(ui->main_stack->count()+1);
            openNewQueryModel(name);
            break;
        }
    }
}

void MainWindow::on_actionOpen_SQL_Tool_triggered()
{
    QString file_name;
    QString query_path;
    SqlTool *sql;

    if (project.getProjectPath().isNull()) {
        query_path = last_path_to_sql;
    } else {
        query_path = project.getProjectPath() + "/scripts/development/review";
    }

    file_name = QFileDialog::getOpenFileName(this, "Open File", query_path,
                                             "Query Tool files (*.qtx);;All files (*.*)");
    if (file_name != "") {

        QFileInfo info(file_name);
        last_path_to_sql = info.path();
        sql = openNewSQLTool();
        sql->restoreGroup(file_name);
        ui->editor_list->currentItem()->setText(sql->getGroupName());
     }
}

void MainWindow::on_actionOpen_Query_Model_triggered()
{
    QueryModel *model;
    QString file_name;
    QFile file;
    QString query_path;

    if (project.getProjectPath().isNull()) {
        query_path = last_path_to_models;
    } else {
        query_path = project.getModelPath();
    }

    file_name = QFileDialog::getOpenFileName(this, "Open File", path_to_models, "Model files (*.xml);;All files (*.*)");
    if (file_name != "") {
        file.setFileName(file_name);
        model = openNewQueryModel("");
        model->openFile(file);
        ui->editor_list->currentItem()->setText(QFileInfo(file).baseName());
    }
}

void MainWindow::on_actionOpen_SQL_File_triggered()
{
    QString file_name;
    QString query_path;
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    QFile file;
    EditorItem *editor;

    if(sql) {

        if (project.getProjectPath().isNull()) {
            query_path = last_path_to_sql;
        } else {
            query_path = project.getProjectPath() + "/scripts/development/review";
        }

        file_name = QFileDialog::getOpenFileName(this, "Open File", query_path,
                                                 "SQL files (*.sql);;All files (*.*)");
        if (file_name != "") {
            file.setFileName(file_name);
            editor = sql->getCurrentEditor();
            if (editor && editor->isModified())
                sql->openFileOnNew(file);
            else
                sql->openFileOnCurrent(file);
        }
    }
}

void MainWindow::on_actionSave_SQL_File_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if(sql) {
        sql->saveCurrent();
    }
}

void MainWindow::on_actionSave_SQL_File_As_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if(sql) {
        sql->saveCurrentAs();
    }
}

void MainWindow::on_actionSave_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    QueryModel *model = dynamic_cast<QueryModel*>(ui->main_stack->currentWidget());
    if (sql) {
        sql->saveGroup();
        ui->editor_list->currentItem()->setText(sql->getGroupName());
    } else if (model)
        model->saveFile();

}

void MainWindow::on_actionSave_As_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    QueryModel *model = dynamic_cast<QueryModel*>(ui->main_stack->currentWidget());
    if (sql) {
        sql->saveGroupAs();
    } else if (model)
        model->saveFileAs();
}

void MainWindow::on_actionSQL_Tool_triggered()
{
    openNewSQLTool(QString("Sql Tool %1").arg(ui->main_stack->count()+1));
}

void MainWindow::on_actionManageConnections_triggered()
{
    DlgConnections *dlg = new DlgConnections(connections, this);
    dlg->exec();

    setConnectionsList();
}

void MainWindow::on_actionDatabase_to_Database_triggered()
{

}

void MainWindow::on_actionAdd_Editor_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if (sql)
        sql->addEditor();
}

void MainWindow::on_actionClose_Editor_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if (sql)
        sql->closeCurrentEditor();
}

void MainWindow::on_actionCreate_Database_triggered()
{

}

void MainWindow::on_actionDrop_Database_triggered()
{

}

void MainWindow::on_actionStart_Transaction_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    DlgTransaction *dlg = new  DlgTransaction(this);

    if (sql) {
        disable_actions();
        if (dlg->exec()) {
            sql->beginTransaction(dlg->getCommand());
        }
        enable_sql_tool_actions(sql);
        enable_sql_transactions(sql);
    }
}

void MainWindow::on_actionRollback_Transaction_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    disable_actions();
    if (sql) {
        sql->rollback();
        enable_sql_tool_actions(sql);
        enable_sql_transactions(sql);
    }
}

void MainWindow::on_actionCommit_Transaction_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    disable_actions();
    if (sql) {
        sql->commit();
        enable_sql_tool_actions(sql);
        enable_sql_transactions(sql);
    }
}

void MainWindow::on_actionManage_Plugins_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    DlgPlugins dlg(interface_list);
    if (sql) {
        dlg.setEditor(sql);
    }
    dlg.exec();
}

void MainWindow::on_main_stack_currentChanged(int arg1)
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->widget(arg1));
    QueryModel *model = dynamic_cast<QueryModel*>(ui->main_stack->currentWidget());
    QTabWidget *tree;
    QListWidget *list;

    disable_actions();

    if (sql) {
        enable_sql_tool_actions(sql);
        enable_sql_transactions(sql);
        ui->output_stack->setCurrentWidget(sql->getOutputSet());

        tree = sql->getPluginWidgetTree();
        list = sql->getFunctionList();

        if (tree)
            ui->plugin_stack->setCurrentWidget(tree);
        if (list)
            ui->function_stack->setCurrentWidget(list);

    }
    if (model){
        enable_model_actions(model);
    }

    ui->actionClose->setEnabled(arg1 != -1);
}

void MainWindow::on_actionConnect_triggered()
{
    PluginTabWidget *ddl_tree;
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    disable_actions();
    if (sql) {
        sql->databaseConnect();
        enable_sql_tool_actions(sql);
    }
}

void MainWindow::on_actionDisconect_triggered()
{
    PluginTabWidget *ddl_tree;
    QListWidget *function_list;

    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    disable_actions();
    if (sql) {
        sql->databaseDisconnect();
        enable_sql_tool_actions(sql);
        ddl_tree = dynamic_cast<PluginTabWidget *>(sql->getPluginWidgetTree());
        function_list = sql->getFunctionList();

        if (ddl_tree) {
            ddl_tree->clearTrees();
        }

        if (function_list)
            function_list->clear();

    }
}

void MainWindow::on_actionConfigurations_triggered()
{
    DlgConfiguration *conf = new DlgConfiguration(!project.getProjectName().isNull(), this);
    conf->setPathToSql(path_to_sql);
    conf->setPathToModels(path_to_models);
    conf->setOptions(cfg_options);
    conf->setTaskPrefix(cfg_task_prefix);
    if (conf->exec()) {
        path_to_sql = conf->getPathToSql();
        path_to_models = conf->getPathToModels();
        cfg_options = conf->getOptions();
        cfg_task_prefix = conf->getTaskPrefix();
    }
}

void MainWindow::on_actionClose_triggered()
{
    QWidget* widget = ui->main_stack->currentWidget();
    SqlTool *sql = dynamic_cast<SqlTool*>(widget);
    QueryModel *model = dynamic_cast<QueryModel*>(widget);
    int row = ui->main_stack->currentIndex();

    if (sql) {
        if(!sql->closeAllEditors()) {
            return;
        } else {
            ui->output_stack->removeWidget(sql->getOutputSet());
            sql->getOutputSet()->deleteLater();

            ui->plugin_stack->removeWidget(sql->getPluginWidgetTree());
            sql->getPluginWidgetTree()->deleteLater();

            ui->function_stack->removeWidget(sql->getFunctionList());
            sql->getFunctionList()->deleteLater();
        }
    }

    if (model) {
        if(!model->canCloseOrReplace())
            return;
    }

    ui->main_stack->removeWidget(widget);
    widget->deleteLater();

    QListWidgetItem *item = ui->editor_list->takeItem(row);
    delete item;

    if (row > 0) {
        row--;
        ui->main_stack->setCurrentIndex(row);
        ui->editor_list->setCurrentRow(row);
    } else if (ui->main_stack->count() > 0) {
        ui->main_stack->setCurrentIndex(0);
        ui->editor_list->setCurrentRow(0);
    }
}

void MainWindow::on_editor_list_currentRowChanged(int currentRow)
{
    ui->main_stack->setCurrentIndex(currentRow);
}

void MainWindow::on_actionExecute_triggered()
{
    OutputSet *out;
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if (sql) {
        out  = sql->getOutputSet();
        switch(out->getType()) {
        case OutputSet::OP_TEXT:
            sql->executeCurrent(new PlainTextOutput(out->getOutput(), ui->message_output, this), ui->show_query->isChecked());
            break;
        case OutputSet::OP_XML:
            sql->executeCurrent(new XMLTextOutput(out->getOutput(), ui->message_output, this), ui->show_query->isChecked());
            break;
        case OutputSet::OP_JSON:
            sql->executeCurrent(new JSONOutput(out->getOutput(), ui->message_output, this), ui->show_query->isChecked());
            break;
        case OutputSet::OP_GRID:
            sql->executeCurrent(new GridOutput(out->getOutput(), ui->message_output, this), ui->show_query->isChecked());
            break;
        case OutputSet::OP_HTML:
            sql->executeCurrent(new HtmlOutput(out->getOutput(), ui->message_output, this), ui->show_query->isChecked());
            break;
        }
    }
}

void MainWindow::on_actionShow_Processes_triggered()
{
    DlgShowProcesses *dlg = new DlgShowProcesses(connections, ui->connection_list->currentRow(), this);
    QString name;

    if (dlg->exec() == QDialog::Accepted) {

    }
}

void MainWindow::on_actionExplain_triggered()
{
    DlgExplain dlg;
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    SciTextOutput *output;

    if (!sql)
        return;

    if (dlg.exec()) {
        QString editor_name = "Explain: " + sql->getGroupName() + " - " + sql->getCurrentEditorName();
        new QListWidgetItem(editor_name, ui->editor_list);
        QsciScintilla *editor = new QsciScintilla(ui->main_stack);
        editor->setReadOnly(true);
        ui->main_stack->addWidget(editor);
        ui->main_stack->setCurrentWidget(editor);
        ui->editor_list->clearSelection();
        ui->editor_list->setCurrentRow(ui->main_stack->currentIndex());
        output = new SciTextOutput(editor, ui->message_output, this);
        output->showBorder(false);
        output->showHeader(false);
        sql->executeCurrent(output, dlg.explain());
    }
}

void MainWindow::on_actionCancel_Current_Query_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if (sql)
        sql->cancelCurrentQuery();
}

void MainWindow::on_connection_list_currentRowChanged(int currentRow)
{
    Q_UNUSED(currentRow) // temporary deactivation
    /*
    if (currentRow != -1) {
        data->databaseDisconnect();
        if(!connections.getConnections().at(currentRow)->isInvalid()) {
            data->databaseConnect(currentRow);

            if (!data->getConnected()) {
                ui->connection_list->item(currentRow)->setText(ui->connection_list->item(currentRow)->text() + " (Invalid)");
                ui->connection_list->item(currentRow)->setForeground(Qt::red);
            }
        }
    }
    */
}

void MainWindow::on_actionScan_Model_Directory_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 path_to_models,
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (dir != "") {
        // order the custom menus to commit suicide :)
        emit clean_custom_menu();

        ui->message_output->clear();
        ui->message_output->moveCursor(QTextCursor::End);
        ModelScanner *scan = new ModelScanner(dir, project);
        connect(scan, &ModelScanner::finished, scan, &ModelScanner::deleteLater);
        connect(scan, &ModelScanner::fileReaded, this, &MainWindow::do_fileReaded);
        connect(scan, &ModelScanner::scanCompleted, this, &MainWindow::do_scanCompleted);
        scan->start();
    }
}

void MainWindow::do_fileReaded(const QString &file_name)
{
    ui->message_output->insertPlainText(file_name+"\n");
}

void MainWindow::do_scanCompleted()
{
    model_data->readModels();
    loadCustomMenus();
}

void MainWindow::on_actionExecute_Saved_Model_triggered()
{
    DlgExecuteModel dlg(model_data->getItems());

    if (dlg.exec())
        executeModel(dlg.selectedFileName());
}

void MainWindow::executeModel(QString resource_name)
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    int currentRow;

    model_data->clear();
    model_data->loadResource(resource_name);

    if(model_data->getOutputType() != "Editor") {
        currentRow = ui->connection_list->currentRow();
    } else {
        if (sql)  {
            currentRow = sql->getSelectedConnectionIndex();
        } else {
            return;
        }
    }

    int item;

    DlgParameters *param = new DlgParameters(model_data,
                                             connections, currentRow, this);

    if (model_data->getParameters().count() > 0 || model_data->getOrders().count() > 0 || model_data->getDatabaseRequest())
        if (!param->exec())
            return;

    if (currentRow != -1) {

        model_data->databaseDisconnect();

        if(!connections.getConnections().at(currentRow)->isInvalid()) {

            model_data->databaseConnect(currentRow, param->getSelectedDatabaseName());

            if (!model_data->getConnected()) {

                ui->connection_list->item(currentRow)->setText(ui->connection_list->item(currentRow)->text() + " (Invalid)");
                ui->connection_list->item(currentRow)->setForeground(Qt::red);

            } else {

                ui->connection_list->item(currentRow)->setText(ui->connection_list->item(currentRow)->text());
                ui->connection_list->item(currentRow)->setForeground(Qt::black);

                if(model_data->getOutputType() != "Editor") {

                    new QListWidgetItem(model_data->getDescription(), ui->editor_list);

#ifdef HML_USE_WEBENGINE
                    QWebEngineView *editor = new QWebEngineView(ui->main_stack);
#endif
#ifndef HML_USE_WEBENGINE
#ifdef HML_USE_WEBKIT
                    QWebView *editor = new QWebView(ui->main_stack);
#endif
#ifndef HML_USE_WEBKIT
                    QTextBrowser *editor = new QTextBrowser(ui->main_stack);
#endif
#endif

                    ui->main_stack->addWidget(editor);
                    ui->main_stack->setCurrentWidget(editor);
                    ui->editor_list->clearSelection();
                    ui->editor_list->setCurrentRow(ui->main_stack->currentIndex());
                    HtmlOutput *list = new HtmlOutput(editor, ui->message_output, this);

                    item = 1;
                    list->getInformationMap()[QString("%1: Connection").arg(item, 2, 10 , QChar('0'))] = ui->connection_list->item(currentRow)->text();
                    item++;
                    if(model_data->getDatabaseRequest())
                        list->getInformationMap()[QString("%1: Database").arg(item, 2, 10 , QChar('0'))] = param->getSelectedDatabaseName();
                    else
                        list->getInformationMap()[QString("%1: Database").arg(item, 2, 10 , QChar('0'))] = "N/A";

                    foreach(QueryParameter *param, model_data->getParameters()) {
                        if (!param->getValue().isNull()) {
                            item++;
                            list->getInformationMap()[QString("%1: ").arg(item, 2, 10 , QChar('0')) + param->getDescription()] = param->getValue();
                        }
                    }

                    model_data->execute(list, ui->show_query->isChecked());

                } else {

                    if(sql) {
                        EditorItem *editor = sql->getCurrentEditor();
                        if (editor) {

                            SciTextOutput *text = new SciTextOutput(editor, ui->message_output, this);
                            text->showBorder(false);
                            text->showHeader(false);
                            text->cleanBefore(false);
                            model_data->execute(text, ui->show_query->isChecked());

                        }

                    }

                }

                model_data->databaseDisconnect();

            }
        }
    }
}

void MainWindow::do_beginExecuteQuery(SqlTool *sender)
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if (sender == sql) {
        ui->actionStart_Transaction->setEnabled(false);
        ui->actionCommit_Transaction->setEnabled(false);
        ui->actionRollback_Transaction->setEnabled(false);
        ui->actionExecute->setEnabled(false);
        ui->actionExplain->setEnabled(false);
        ui->actionCancel_Current_Query->setEnabled(false);
        ui->actionPlanner_Method_Settings->setEnabled(false);
        ui->actionGenetic_Query_Optimization_Settings->setEnabled(false);
        ui->actionPallelism_Settings->setEnabled(false);
        ui->actionOther_Planner_Related_Settings->setEnabled(false);
        ui->actionWorkers_Settings->setEnabled(false);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconect->setEnabled(false);
        ui->actionCancel_Current_Query->setEnabled(true);
    }
}

void MainWindow::do_endExecuteQuery(SqlTool *sender)
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if (sender == sql) {
        if (sql->getMode() == SqlTool::MODE_QUERY) {
            if (sql->transaction()) {
                ui->actionCommit_Transaction->setEnabled(true);
                ui->actionRollback_Transaction->setEnabled(true);
            } else {
                ui->actionStart_Transaction->setEnabled(true);
            }
        }
        ui->actionExecute->setEnabled(true);
        ui->actionExplain->setEnabled(true);
        ui->actionPlanner_Method_Settings->setEnabled(true);
        ui->actionGenetic_Query_Optimization_Settings->setEnabled(true);
        ui->actionPallelism_Settings->setEnabled(true);
        ui->actionOther_Planner_Related_Settings->setEnabled(true);
        ui->actionWorkers_Settings->setEnabled(true);
        ui->actionConnect->setEnabled(true);
        ui->actionDisconect->setEnabled(true);
        ui->actionCancel_Current_Query->setEnabled(false);
    }
}

void MainWindow::loadCustomMenus()
{
    QMenu *root = ui->menuUser_Models;
    QMenu *menu_element;
    QMenu *next_element;
    QAction *child;
    QSignalMapper *mapper = new QSignalMapper(this);

    foreach(ModelItem *item, model_data->getItems()) {
        menu_element = root;
        QStringList parts = item->getMenuParts();
        for(int i = 0; i < parts.count(); i++ ) {
            next_element = nullptr;
            foreach(child, menu_element->actions()) {
                if (child->menu()) {
                    if (child->menu()->title() == parts.at(i)) {
                        next_element = child->menu();
                        menu_element->addMenu(next_element);                       
                        menu_element = next_element;
                    }
                }
            }
            if (!next_element) {
                next_element = new QMenu(item->getMenuParts().at(i));
                menu_element->addMenu(next_element);
                connect(this, SIGNAL(clean_custom_menu()), next_element, SLOT(deleteLater()));
                menu_element = next_element;
            }
            if(i == parts.count() - 1) {
                child = new QAction(item->getDescription());
                connect(child, SIGNAL(triggered()), mapper, SLOT(map()));
                connect(this, SIGNAL(clean_custom_menu()), child, SLOT(deleteLater()));
                mapper->setMapping(child, item->getFileName());
                menu_element->addAction(child);
            }
        }
    }

    connect(mapper, SIGNAL(mapped(QString)), this, SLOT(executeModel(QString)));
}

void MainWindow::on_actionPlanner_Method_Settings_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());

    if (sql)
        sql->plannerMethodSettings();
}

void MainWindow::on_actionGenetic_Query_Optimization_Settings_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if(sql)
        sql->geneticQueryOptimizationsSettings();
}

void MainWindow::on_actionPallelism_Settings_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());

    if(sql)
        sql->parallelismSettings();
}

void MainWindow::on_actionOther_Planner_Related_Settings_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());

    if(sql)
        sql->otherPlanningSettings();
}

void MainWindow::on_actionWorkers_Settings_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());

    if(sql)
        sql->workersSettings();
}

void MainWindow::on_actionIndexes_Sizes_triggered()
{
    executeModel(":/query_defs/query_models/index_sizes.xml");
}

void MainWindow::on_actionIndexes_Bloat_triggered()
{
    executeModel(":/query_defs/query_models/index_bloat.xml");
}

void MainWindow::on_actionStat_All_Indexes_triggered()
{
    executeModel(":/query_defs/query_models/stat_all_indexes.xml");
}

void MainWindow::on_actionUseless_Indexes_triggered()
{
    executeModel(":/query_defs/query_models/useless_indexes.xml");
}

void MainWindow::on_actionWasted_Index_Space_triggered()
{
    executeModel(":/query_defs/query_models/wasted_index_space.xml");
}

void MainWindow::on_actionDatabases_triggered()
{
    executeModel(":/query_defs/query_models/database_info.xml");
}

void MainWindow::on_actionDatabases_Statistics_triggered()
{
    executeModel(":/query_defs/query_models/database_statistics.xml");
}

void MainWindow::on_actionDatabase_Conflicts_triggered()
{
    executeModel(":/query_defs/query_models/database_conflicts.xml");
}

void MainWindow::on_actionCache_Hit_Ratio_triggered()
{
    executeModel(":/query_defs/query_models/database_cache_hit_ratio.xml");
}

void MainWindow::on_actionBlocking_Processes_triggered()
{
    executeModel(":/query_defs/query_models/database_blocking_processes.xml");
}

void MainWindow::on_actionBlocking_Process_With_Applications_triggered()
{
    executeModel(":/query_defs/query_models/database_blocking_processes_application.xml");
}

void MainWindow::on_actionList_of_Tables_triggered()
{
    executeModel(":/query_defs/query_models/struct_list_of_tables.xml");
}

void MainWindow::on_actionSchemas_triggered()
{
    executeModel(":/query_defs/query_models/struct_schemas.xml");
}

void MainWindow::on_actionTCP_Settings_triggered()
{
    executeModel(":/query_defs/query_models/tcp_settings.xml");
}

void MainWindow::on_actionAutovacuum_Settings_triggered()
{
    executeModel(":/query_defs/query_models/autovacuum_settings.xml");
}

void MainWindow::on_actionLog_Settings_triggered()
{
    executeModel(":/query_defs/query_models/log_settings.xml");
}

void MainWindow::on_actionLanguage_Settings_triggered()
{
    executeModel(":/query_defs/query_models/language_settings.xml");
}

void MainWindow::on_actionAll_Other_Settings_triggered()
{
    executeModel(":/query_defs/query_models/all_other_settings.xml");
}

void MainWindow::on_actionSSL_Settings_triggered()
{
    executeModel(":/query_defs/query_models/ssl_settings.xml");
}

void MainWindow::on_actionNormal_Functions_triggered()
{
    executeModel(":/query_defs/query_models/normal_functions.xml");
}

void MainWindow::on_actionTrigger_Functions_By_Schema_triggered()
{
    executeModel(":/query_defs/query_models/trigger_functions.xml");
}

void MainWindow::on_actionAll_Functions_By_Schema_triggered()
{
    executeModel(":/query_defs/query_models/all_functions.xml");
}

void MainWindow::on_actionList_of_Views_triggered()
{
    executeModel(":/query_defs/query_models/struct_list_of_views.xml");
}

void MainWindow::on_actionList_of_Table_Fields_triggered()
{
    executeModel(":/query_defs/query_models/struct_list_table_fields.xml");
}

void MainWindow::on_actionNew_Project_triggered()
{
    DlgProject prj(project, connections);

    if (prj.projectAdd()) {

        project.saveConnections();
        project.writeConfig();

        connections.clear();
        setWindowTitle("PostgreSQL DBA Tool - " + project.getProjectName());
        connections.readConnections(project.getProjectPath() + "/config");
        path_to_sql = project.getQueryPath();
        last_path_to_sql = path_to_sql;
        path_to_models = project.getModelPath();
        last_path_to_models = path_to_models;
        setConnectionsList();

    }
}

void MainWindow::on_actionProject_Options_triggered()
{
    DlgProject prj(project, connections);

    if (prj.projectEdit()) {

        project.saveConnections();
        project.writeConfig();

        connections.clear();

        setWindowTitle("PostgreSQL DBA Tool - " + project.getProjectName());
        connections.readConnections(project.getProjectPath() + "/config");
        path_to_sql = project.getQueryPath();
        last_path_to_sql = path_to_sql;
        path_to_models = project.getModelPath();
        last_path_to_models = path_to_models;
        setConnectionsList();
    }
}

void MainWindow::on_actionClose_Project_triggered()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

    if (!maybeSave())
        return;

    // Save Connections and config
    project.saveConnections();
    project.writeConfig();

    project.clean();
    connections.clear();

    // read default connections
    connections.readConnections();
    setConnectionsList();

    last_path_to_sql = path_to_sql;
    path_to_models = settings.value("path_to_models", "").toString();

    setWindowTitle("PostgreSQL DBA Tool");
}

void MainWindow::on_actionOpen_Project_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Project Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {

        if (QFileInfo(dir+"/config/config.xml").exists()) {

            // Save connections and setings from the opened project if it is the case.
            if(project.getProjectName() != "") {
                project.saveConnections();
                project.writeConfig();
            }

            project.setProjectPath(dir);
            project.readConfig();
            connections.clear();
            setWindowTitle("PostgreSQL DBA Tool - " + project.getProjectName());
            connections.readConnections(project.getProjectPath() + "/config");
            path_to_sql = project.getQueryPath();
            last_path_to_sql = path_to_sql;
            path_to_models = project.getModelPath();
            setConnectionsList();
        }

    }
}

void MainWindow::on_actionCreate_New_SQL_Script_triggered()
{
    DlgCreateScript dlg(project.getProjectPath()+"/scripts/development/run/");
    SqlTool *sql_tool;
    QFile file;

    if (dlg.exec()) {
        sql_tool = openNewSQLTool("sql");

        file.setFileName(dlg.getFileName());
        sql_tool->openFileOnCurrent(file);
        ui->editor_list->currentItem()->setText(dlg.getDescription());
    }
}

void MainWindow::on_actionUndo_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if(sql) {
        sql->undo();
    }
}

void MainWindow::on_actionRedo_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if(sql) {
        sql->redo();
    }
}

void MainWindow::on_actionCut_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if(sql) {
        sql->cut();
    }
}

void MainWindow::on_actionCopy_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if(sql) {
        sql->copy();
    }
}

void MainWindow::on_actionPaste_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if(sql) {
        sql->paste();
    }
}

void MainWindow::on_actionDelete_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if(sql) {
        sql->deleteText();
    }
}

void MainWindow::on_actionSelect_All_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if(sql) {
        sql->selectAll();
    }
}

void MainWindow::on_actionFind_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if(sql) {
        sql->findFirst();
    }
}

void MainWindow::on_actionFind_Next_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if(sql) {
        sql->findNext();
    }
}

void MainWindow::on_actionExecute_Project_Script_triggered()
{
    FrmProjectScript *frm;

    frm = new FrmProjectScript(project, ui->main_stack);

    new QListWidgetItem("Project Runner", ui->editor_list);
    ui->main_stack->addWidget(frm);
    ui->main_stack->setCurrentWidget(frm);
    ui->editor_list->clearSelection();
    ui->editor_list->setCurrentRow(ui->main_stack->currentIndex());
    connect(frm, SIGNAL(openFileOnSQLTool(QString, int)), this, SLOT(openScriptOnNewTool(QString, int)));
}

void MainWindow::sqlToolModeChanged(SqlTool *sql, int current_mode)
{
    Q_UNUSED(current_mode)
     enable_sql_tool_actions(sql);
     // always rollback transaction if change mode
     if (sql->transaction()) {
         sql->rollback();
     }
}

void MainWindow::closeRequested()
{
    on_actionClose_triggered();

    FrmProjectScript *frm;

    for (int i = 0; i < ui->main_stack->count(); i++) {

        frm = dynamic_cast<FrmProjectScript*>(ui->main_stack->widget(i));
        if(frm) {
            frm->refreshAll();
        }
    }

}

void MainWindow::openScriptOnNewTool(QString file_name, int internal_mode)
{
    QFile file(file_name);
    SqlTool *sql = openNewSQLTool("Run Script", internal_mode);
    sql->setMode(internal_mode);
    sql->openFileOnCurrent(file);
}


void MainWindow::on_actionViewMaintenance_Connections_toggled(bool arg1)
{
    ui->dock_connections->setVisible(arg1);
}

void MainWindow::on_actionViewSql_Editors_List_toggled(bool arg1)
{
    ui->doc_sql_editors->setVisible(arg1);
}

void MainWindow::on_actionViewOutputs_toggled(bool arg1)
{
    ui->dock_output->setVisible(arg1);
}

void MainWindow::on_actionViewMessages_toggled(bool arg1)
{
    ui->dock_messages->setVisible(arg1);
}

void MainWindow::on_actionViewCode_Function_toggled(bool arg1)
{
    ui->dock_function->setVisible(arg1);
}

void MainWindow::on_actionViewCode_Generators_toggled(bool arg1)
{
    ui->dock_plugin->setVisible(arg1);
}

void MainWindow::on_bt_execute_clicked()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if(sql) {
        sql->executeSelectedFunction();
    }
}


