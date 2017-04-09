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
    conn = NULL;
    status = CONNECTION_BAD;
    loadDatabases();
}

MainWindow::~MainWindow()
{
    delete ui;
    if (conn)
        PQfinish(conn);
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

void MainWindow::loadDatabases()
{
    const char *keys[] = { "host", "user", "password", "dbname", NULL };
    const char *values[] = { "host-name", "user-name", "user-pass","postgres", NULL };
    PGresult   *res;
    int nFields;

    conn = PQconnectdbParams(keys, values, 0);

    status = PQstatus(conn);

    if (status == CONNECTION_OK) {

        res = PQexec(conn,"BEGIN");
        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            QMessageBox::critical(this, tr("Error"),QString(tr("BEGIN command failed: %1")).arg(PQerrorMessage(conn)));
            PQclear(res);
            return;
        }
        PQclear(res);
        res = PQexec(conn, "DECLARE dbn CURSOR FOR select datname from pg_database");
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
        {
            QMessageBox::critical(this, tr("Error"),QString(tr("DECLARE CURSOR failed: %1")).arg(PQerrorMessage(conn)));
            PQclear(res);
            return;
        }
        PQclear(res);

        res = PQexec(conn, "FETCH ALL in dbn");
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            QMessageBox::critical(this, tr("Error"),QString(tr("FECHALL failed: %1")).arg(PQerrorMessage(conn)));
            PQclear(res);
            return;
        }
        nFields = PQnfields(res);
        //for (int i = 0; i < nFields; i++)
        //    qDebug() << PQfname(res, i);
        for (int i = 0; i < PQntuples(res); i++) {
            for (int j = 0; j < nFields; j++)
                ui->db_list->addItem(new QListWidgetItem(PQgetvalue(res, i, j)));
        }
        ui->db_list->item(0)->setSelected(true);

        PQclear(res);

        res = PQexec(conn, "CLOSE dbn");
        PQclear(res);

        res = PQexec(conn, "END");
        PQclear(res);

    } else {
        QMessageBox::critical(this, tr("Error"), QString(tr("Error opening database %1")).arg( PQerrorMessage(conn)));
    }
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
