#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>

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
    QSettings settings("Qvaliz", "pg-dba-tools");
    settings.setValue("geometry", saveGeometry());
    QWidget::closeEvent(event);
}

void MainWindow::showEvent(QShowEvent *event)
{
    QSettings settings("Qvaliz", "pg-dba-tools");
    restoreGeometry(settings.value("geometry").toByteArray());
}

void MainWindow::on_actionExit_triggered()
{
    close();
}
