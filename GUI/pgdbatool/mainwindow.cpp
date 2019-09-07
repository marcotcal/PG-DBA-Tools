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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    readSettings();    
    disable_actions();
    setConnectionsTree();
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
}

void MainWindow::enable_sql_tool_actions(SqlTool *sql)
{    
    ui->actionAdd_Editor->setEnabled(true);
    ui->actionClose_Editor->setEnabled(true);

    ui->actionConnect->setEnabled(!sql->connected());
    ui->actionDisconect->setEnabled(sql->connected());
    if (sql->connected()) {
        ui->actionStart_Transaction->setEnabled(true);
        ui->actionCommit_Transaction->setEnabled(false);
        ui->actionRollback_Transaction->setEnabled(false);
    } else {
        ui->actionStart_Transaction->setEnabled(false);
        ui->actionCommit_Transaction->setEnabled(false);
        ui->actionRollback_Transaction->setEnabled(false);
    }
}

void MainWindow::enable_sql_transactions(SqlTool *sql) {
    ui->actionStart_Transaction->setEnabled(!sql->transaction());
    ui->actionCommit_Transaction->setEnabled(sql->transaction());
    ui->actionRollback_Transaction->setEnabled(sql->transaction());
}

void MainWindow::setConnectionsTree()
{
    ui->connection_list->clear();
    QTreeWidgetItem *item;
    for (int i = 0; i < connections.getConnections().count(); i++) {
        ConnectionElement *conn = connections.getConnections().at(i);
        QStringList columns;
        columns << conn->name();
        item = new QTreeWidgetItem(ui->connection_list, columns);        
    }
    if (ui->connection_list->topLevelItem(0))
        ui->connection_list->setCurrentItem(ui->connection_list->topLevelItem(0), 0, QItemSelectionModel::Select);
}

void MainWindow::openNewSQLTool()
{
    SqlTool *sql;
    bool ok;
    QTreeWidgetItem *tree_item;
    QStringList columns;
    QString text;

    text = QString("New Sql Tool %1").arg(ui->main_stack->count()+1);

    text = QInputDialog::getText(this, tr("New SQL Tool Set"),
                                             tr("Set Name:"), QLineEdit::Normal,
                                             text, &ok);

    if (ok && !text.isEmpty()) {

        columns.append(text);

        tree_item = new QTreeWidgetItem(ui->editor_list, columns);
        sql = new SqlTool(ui->main_stack);
        ui->main_stack->addWidget(sql);
        ui->main_stack->setCurrentWidget(sql);
        tree_item->setData(0, Qt::UserRole, QVariant(ui->main_stack->currentIndex()));
        ui->editor_list->clearSelection();
        tree_item->setSelected(true);
    }
}

void MainWindow::openNewQueryModel()
{
    QueryModel *qmod;
    QTreeWidgetItem *tree_item;
    bool ok;
    QStringList columns;
    QString text;

    text = QString("New Query Model %1").arg(ui->main_stack->count()+1);

    text = QInputDialog::getText(this, tr("New Query Model"),
                                             tr("Qeury Model Name:"), QLineEdit::Normal,
                                             text, &ok);

    if (ok && !text.isEmpty()) {

        columns.append(text);

        tree_item = new QTreeWidgetItem(ui->editor_list, columns);
        qmod = new QueryModel(ui->main_stack);
        ui->main_stack->addWidget(qmod);
        ui->main_stack->setCurrentWidget(qmod);
        tree_item->setData(0, Qt::UserRole, QVariant(ui->main_stack->currentIndex()));
        ui->editor_list->clearSelection();
        tree_item->setSelected(true);
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionNew_triggered()
{
    DlgMenuNew *dlg = new DlgMenuNew(this);

    if (dlg->exec() == QDialog::Accepted) {

        switch(dlg->getSelection()) {
        case 0:
            openNewSQLTool();
            break;
        case 1:
            openNewQueryModel();
            break;
        }
    }
}

void MainWindow::on_actionOpen_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    QueryModel *model = dynamic_cast<QueryModel*>(ui->main_stack->currentWidget());

    if (sql) {
        sql->openFileOnCurrent();
    } else if (model) {
        model->openFile();
    } else {
        QString file_name;
        file_name = QFileDialog::getOpenFileName(this, "Open File", "./", "SQL files (*.sql);;Model files (*.xml);;All files (*.*))");
    }
}

void MainWindow::on_actionSave_triggered()
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->currentWidget());
    QueryModel *model = dynamic_cast<QueryModel*>(ui->main_stack->currentWidget());


}

void MainWindow::on_actionSave_As_triggered()
{

}

void MainWindow::on_actionSQL_Tool_triggered()
{
    openNewSQLTool();
}

void MainWindow::on_actionManageConnections_triggered()
{
    DlgConnections *dlg = new DlgConnections(connections, this);
    dlg->exec();
    setConnectionsTree();
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
    disable_actions();
    if (sql) {
        sql->beginTransaction();
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

void MainWindow::on_editor_list_itemActivated(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    ui->main_stack->setCurrentIndex(item->data(0, Qt::UserRole).toInt());
}

void MainWindow::on_main_stack_currentChanged(int arg1)
{
    SqlTool *sql = dynamic_cast<SqlTool*>(ui->main_stack->widget(arg1));

    disable_actions();

    if (sql) {
        enable_sql_tool_actions(sql);
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
