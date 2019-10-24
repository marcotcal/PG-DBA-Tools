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
#include <QWebEngineView>
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readSettings();    
    disable_actions();
    data = new QueryModelData(connections, this);
    data->readModels();
    loadCustomMenus();
    setConnectionsList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::maybeSave()
{
    SqlTool *sql;

    for (int i = 0; i < ui->main_stack->count(); i++) {
        ui->main_stack->setCurrentIndex(i);
        sql = dynamic_cast<SqlTool*>(ui->main_stack->widget(i));
        if (sql) {
            if(!sql->closeAllEditors())
                return false;
        }
    }

    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
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
    path_to_models = settings.value("path_to_models", "").toString();
    restoreState(settings.value("DOCK_LOCATIONS").toByteArray(),1);
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
    connections.readConnections();
}

void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("path_to_models", path_to_models);
    settings.setValue("path_to_sql", path_to_sql);
    settings.setValue("DOCK_LOCATIONS",this->saveState(1));
    connections.writeConnections();
}

void MainWindow::disable_actions()
{
    ui->actionSave->setEnabled(false);
    ui->actionSave_As->setEnabled(false);
    ui->actionAdd_Editor->setEnabled(false);
    ui->actionClose_Editor->setEnabled(false);
    ui->actionStart_Transaction->setEnabled(false);
    ui->actionCommit_Transaction->setEnabled(false);
    ui->actionRollback_Transaction->setEnabled(false);
    ui->actionExecute->setEnabled(false);
    ui->actionOpen_SQL_Tool_Set->setEnabled(false);
    ui->actionSave_SQL_Tool->setEnabled(false);
    ui->actionSave_SQL_Tool_As->setEnabled(false);
    ui->actionExplain->setEnabled(false);
    ui->actionConnect->setEnabled(false);
    ui->actionDisconect->setEnabled(false);
    ui->actionCancel_Current_Query->setEnabled(false);
}

void MainWindow::enable_sql_tool_actions(SqlTool *sql)
{    
    ui->actionAdd_Editor->setEnabled(true);
    ui->actionClose_Editor->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionSave_As->setEnabled(true);

    ui->actionOpen_SQL_Tool_Set->setEnabled(true);
    ui->actionSave_SQL_Tool->setEnabled(true);
    ui->actionSave_SQL_Tool_As->setEnabled(true);

    ui->actionConnect->setEnabled(!sql->connected() && !sql->isRunning());
    ui->actionDisconect->setEnabled(sql->connected() && !sql->isRunning());
    // implicity transactions
    ui->actionExecute->setEnabled(sql->connected()  && !sql->isRunning());
    ui->actionExplain->setEnabled(sql->connected()  && !sql->isRunning());

    ui->actionCancel_Current_Query->setEnabled(sql->isRunning());

    if (sql->connected()) {
        ui->actionStart_Transaction->setEnabled(!sql->isRunning());
        ui->actionCommit_Transaction->setEnabled(false);
        ui->actionRollback_Transaction->setEnabled(false);     
    } else {
        ui->actionStart_Transaction->setEnabled(false);
        ui->actionCommit_Transaction->setEnabled(false);
        ui->actionRollback_Transaction->setEnabled(false);
    }
    ui->actionSave->setEnabled(true);
    ui->actionSave_As->setEnabled(true);
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
    ui->actionStart_Transaction->setEnabled(!sql->transaction() && !sql->isRunning());
    ui->actionCommit_Transaction->setEnabled(sql->transaction() && !sql->isRunning());
    ui->actionRollback_Transaction->setEnabled(sql->transaction() && !sql->isRunning());
    // suport implicit transactions
    //ui->actionExecute->setEnabled(sql->transaction());
    ui->actionExecute->setEnabled(sql->connected() && !sql->isRunning());
    ui->actionExplain->setEnabled(sql->connected() && !sql->isRunning());
    ui->actionSave->setEnabled(true);
    ui->actionSave_As->setEnabled(true);
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

SqlTool *MainWindow::openNewSQLTool(QString name)
{
    SqlTool *sql;
    OutputSet *out;

    if (!name.isEmpty()) {

        new QListWidgetItem(name, ui->editor_list);
        sql = new SqlTool(connections, ui->main_stack);
        ui->main_stack->addWidget(sql);
        ui->main_stack->setCurrentWidget(ui->main_stack);
        ui->editor_list->clearSelection();        
        sql->setGroupName(name);
        out = new OutputSet(sql);
        sql->setOutputSet(out);
        ui->output_stack->addWidget(out);
        ui->output_stack->setCurrentWidget(out);
        ui->main_stack->setCurrentWidget(sql);
        ui->editor_list->setCurrentRow(ui->main_stack->currentIndex());
        connect(sql, SIGNAL(beginExecution(SqlTool *)), this, SLOT(do_beginExecuteQuery(SqlTool*)));
        connect(sql, SIGNAL(endExecution(SqlTool *)), this, SLOT(do_endExecuteQuery(SqlTool*)));
        return sql;

    }
    return nullptr;
}

QueryModel *MainWindow::openNewQueryModel(QString name)
{
    QueryModel *qmod;

    if (!name.isEmpty()) {

        new QListWidgetItem(name, ui->editor_list);
        qmod = new QueryModel(connections, ui->main_stack);
        ui->main_stack->addWidget(qmod);
        ui->main_stack->setCurrentWidget(qmod);        
        ui->editor_list->clearSelection();
        ui->editor_list->setCurrentRow(ui->main_stack->currentIndex());

        return qmod;
    }
    return nullptr;
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

void MainWindow::on_actionOpen_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    QueryModel *model = dynamic_cast<QueryModel*>(ui->main_stack->currentWidget());
    QString file_name;
    QFile file;

    if (sql) {
        file_name = QFileDialog::getOpenFileName(this, "Open File", path_to_sql, "SQL files (*.sql);;All files (*.*))");
        if (file_name != "") {
            file.setFileName(file_name);
            sql->openFileOnCurrent(file);            
            ui->editor_list->currentItem()->setText(QString("Sql Tool %1").arg(ui->main_stack->count()+1));
        }
    } else if (model) {
        file_name = QFileDialog::getOpenFileName(this, "Open File", path_to_models, "Model files (*.xml);;All files (*.*))");
        if (file_name != "") {
            file.setFileName(file_name);
            model->openFile(file);
            ui->editor_list->currentItem()->setText(QFileInfo(file).baseName());
        }
    } else {
        file_name = QFileDialog::getOpenFileName(this, "Open File", path_to_sql,
                                                 "SQL files (*.sql);;Model files (*.xml);;All files (*.*))");
        if (file_name != "") {
            QFile file(file_name);
            if (QFileInfo(file).suffix().toLower() == "sql") {
                    sql = openNewSQLTool(QString("Sql Tool %1").arg(ui->main_stack->count()+1));
                    sql->openFileOnCurrent(file);
            } else {
                model = openNewQueryModel(QFileInfo(file).baseName());
                model->openFile(file);
            }
         }
    }
}

void MainWindow::on_actionSave_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    QueryModel *model = dynamic_cast<QueryModel*>(ui->main_stack->currentWidget());
    if (sql)
        sql->saveCurrent();
    else if (model)
        model->saveFile();

}

void MainWindow::on_actionSave_As_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    QueryModel *model = dynamic_cast<QueryModel*>(ui->main_stack->currentWidget());
    if (sql)
        sql->saveCurrentAs();
    else if (model)
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

void MainWindow::on_actionSave_SQL_Tool_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if (sql) {
        sql->saveGroup();
        ui->editor_list->currentItem()->setText(sql->getGroupName());
    }
}

void MainWindow::on_actionSave_SQL_Tool_As_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if (sql) {
        sql->saveGroupAs();
        ui->editor_list->currentItem()->setText(sql->getGroupName());
    }
}

void MainWindow::on_actionOpen_SQL_Tool_Set_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if (sql) {
        sql->restoreGroup();
        ui->editor_list->currentItem()->setText(sql->getGroupName());
    }
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

}

void MainWindow::on_main_stack_currentChanged(int arg1)
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->widget(arg1));
    QueryModel *model = dynamic_cast<QueryModel*>(ui->main_stack->currentWidget());

    disable_actions();

    if (sql) {
        enable_sql_tool_actions(sql);
        enable_sql_transactions(sql);
        ui->output_stack->setCurrentWidget(sql->getOutputSet());
    }
    if (model){
        enable_model_actions(model);
    }
}

void MainWindow::on_actionConnect_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    disable_actions();
    if (sql) {
        sql->databaseConnect();
        enable_sql_tool_actions(sql);
    }
}

void MainWindow::on_actionDisconect_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    disable_actions();
    if (sql) {
        sql->databaseDisconnect();
        enable_sql_tool_actions(sql);
    }
}

void MainWindow::on_actionConfigurations_triggered()
{
    DlgConfiguration *conf = new DlgConfiguration(this);
    conf->setPathToSql(path_to_sql);
    conf->setPathToModels(path_to_models);
    if (conf->exec()) {
        path_to_sql = conf->getPathToSql();
        path_to_models = conf->getPathToModels();
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
        ModelScanner *scan = new ModelScanner(dir);
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
    data->readModels();
    loadCustomMenus();
}

void MainWindow::on_actionExecute_Saved_Model_triggered()
{
    DlgExecuteModel dlg(data->getItems());

    if (dlg.exec())
        executeModel(dlg.selectedFileName());
}

void MainWindow::executeModel(QString resource_name)
{
    QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    data->clear();
    data->loadResource(resource_name);

    DlgParameters *param = new DlgParameters(data, this);

    if ((data->getParameters().count() == 0 && data->getOrders().count() == 0) || param->exec()) {

        new QListWidgetItem(data->getDescription(), ui->editor_list);
        QWebEngineView *editor = new QWebEngineView(ui->main_stack);
        ui->main_stack->addWidget(editor);
        ui->main_stack->setCurrentWidget(editor);
        ui->editor_list->clearSelection();
        ui->editor_list->setCurrentRow(ui->main_stack->currentIndex());        
        data->execute(new HtmlOutput(editor, ui->message_output, this), ui->show_query->isChecked());   
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
        ui->actionConnect->setEnabled(false);
        ui->actionDisconect->setEnabled(false);
        ui->actionCancel_Current_Query->setEnabled(true);
    }
}

void MainWindow::do_endExecuteQuery(SqlTool *sender)
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if (sender == sql) {
        if (sql->transaction()) {
            ui->actionCommit_Transaction->setEnabled(true);
            ui->actionRollback_Transaction->setEnabled(true);
        } else {
            ui->actionStart_Transaction->setEnabled(true);
        }
        ui->actionExecute->setEnabled(true);
        ui->actionExplain->setEnabled(true);
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

    foreach(ModelItem *item, data->getItems()) {
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
