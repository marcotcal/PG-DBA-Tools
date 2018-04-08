#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QDebug>
#include <QMessageBox>
#include <dlgconnections.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);       
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{

}

void MainWindow::showEvent(QShowEvent *event)
{
    QSettings settings("Qvaliz", "pg-dba-tools");
    restoreGeometry(settings.value("geometry").toByteArray());
}

DMOConnection *MainWindow::getDatabase() const
{
    return database;
}

void MainWindow::setDatabase(DMOConnection *value)
{
    database = value;
}

void MainWindow::loadDatabases(void)
{
    foreach(QString str, database->getDatabases()) {
        ui->db_list->addItem(new QListWidgetItem(str));
    }
    ui->db_list->item(0)->setSelected(true);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionConnections_triggered()
{
    DlgConnections dlg;
    dlg.exec();
}
