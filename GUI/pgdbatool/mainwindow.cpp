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
#include "dlgmenunew.h"
#include "dlgconnections.h"
#include "dlgconfiguration.h"
#include "plaintextoutput.h"
#include "xmltextoutput.h"
#include "jsonoutput.h"
#include "htmloutput.h"
#include "gridoutput.h"
#include "dlgtransaction.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    readSettings();    
    disable_actions();
    setConnectionsList();
    ui->output_stack->setCurrentIndex(0);
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
}

void MainWindow::enable_sql_tool_actions(SqlTool *sql)
{    
    ui->actionAdd_Editor->setEnabled(true);
    ui->actionClose_Editor->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionSave_As->setEnabled(true);

    ui->actionConnect->setEnabled(!sql->connected());
    ui->actionDisconect->setEnabled(sql->connected());
    // implicity transactions
    ui->actionExecute->setEnabled(sql->connected());
    if (sql->connected()) {
        ui->actionStart_Transaction->setEnabled(true);
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
    ui->actionStart_Transaction->setEnabled(!sql->transaction());
    ui->actionCommit_Transaction->setEnabled(sql->transaction());
    ui->actionRollback_Transaction->setEnabled(sql->transaction());
    // suport implicit transactions
    //ui->actionExecute->setEnabled(sql->transaction());
    ui->actionExecute->setEnabled(sql->connected());
    ui->actionSave->setEnabled(true);
    ui->actionSave_As->setEnabled(true);
}

void MainWindow::setConnectionsList()
{
    ui->connection_list->clear();
    for (int i = 0; i < connections.getConnections().count(); i++) {
        ConnectionElement *conn = connections.getConnections().at(i);
        new QListWidgetItem(conn->name(), ui->connection_list);
    }
    if (ui->connection_list->count() > 0)
        ui->connection_list->setCurrentRow(0);
}

SqlTool *MainWindow::openNewSQLTool(QString name)
{
    SqlTool *sql;

    if (!name.isEmpty()) {

        new QListWidgetItem(name, ui->editor_list);
        sql = new SqlTool(connections, ui->main_stack);
        ui->main_stack->addWidget(sql);
        ui->main_stack->setCurrentWidget(sql);        
        ui->editor_list->clearSelection();
        ui->editor_list->setCurrentRow(ui->main_stack->currentIndex());

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
        enable_sql_transactions(sql);
    }
}

void MainWindow::on_actionRollback_Transaction_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    disable_actions();
    if (sql) {
        sql->rollback();
        enable_sql_transactions(sql);
    }
}

void MainWindow::on_actionCommit_Transaction_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    disable_actions();
    if (sql) {
        sql->commit();
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
        if(!sql->closeAllEditors())
            return;
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

void MainWindow::on_bt_txt_clicked()
{
    ui->output_stack->setCurrentIndex(0);

}

void MainWindow::on_bt_xml_clicked()
{
    ui->output_stack->setCurrentIndex(0);
}

void MainWindow::on_bt_json_clicked()
{
    ui->output_stack->setCurrentIndex(0);
}

void MainWindow::on_bt_grid_clicked()
{
    ui->output_stack->setCurrentIndex(1);
}

void MainWindow::on_bt_html_clicked()
{
    ui->output_stack->setCurrentIndex(2);
}

void MainWindow::on_actionExecute_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    if (sql) {
        switch(ui->output_stack->currentIndex()) {
        case 0:
            if (ui->bt_txt->isChecked())
                sql->executeCurrent(new PlainTextOutput(ui->text_output, ui->message_output, this));
            else if (ui->bt_xml->isChecked())
                sql->executeCurrent(new XMLTextOutput(ui->text_output, ui->message_output, this));
            else if (ui->bt_json)
                sql->executeCurrent(new JSONOutput(ui->text_output, ui->message_output, this));
            break;
        case 1:
            sql->executeCurrent(new GridOutput(ui->grid_output, ui->message_output, this));
            break;
        case 2:
            sql->executeCurrent(new HtmlOutput(ui->html_output, ui->message_output, this));
            break;
        }
    }

}
