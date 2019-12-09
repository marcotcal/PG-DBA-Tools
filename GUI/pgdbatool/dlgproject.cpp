#include "dlgproject.h"
#include "ui_dlgproject.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

DlgProject::DlgProject(ConnectionsData &conn, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgProject),
    connections(conn)
{
    ui->setupUi(this);
    loadConnections(ui->cb_development_connection);
    loadConnections(ui->cb_staging_connection);
    loadConnections(ui->cb_production_connection);
}

DlgProject::~DlgProject()
{
    delete ui;
}

void DlgProject::createProject()
{

    // create skeleton
    createSkeleton(ui->ed_project_path->text());

    QString config = "# PGDBA Tools Config File\n\n";
    QString file_name = ui->ed_project_path->text() + "/config/config.txt";

    config += QString("project_name=%1\n").
            arg(ui->ed_project_name->text());
    config += QString("development_connection=%1\n").
            arg(ui->cb_development_connection->itemText(
                    ui->cb_development_connection->currentIndex()));
    config += QString("staging_connection=%1\n").
            arg(ui->cb_staging_connection->itemText(
                    ui->cb_staging_connection->currentIndex()));
    config += QString("prodution_connection=%1\n").
            arg(ui->cb_production_connection->itemText(
                    ui->cb_production_connection->currentIndex()));
    config += QString("query_dir=%1\n").
            arg(ui->ed_query_dir->text());
    config += QString("models_dir=%1\n").
            arg(ui->ed_models_dir->text());


    QFile file(file_name);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream << config;
        file.close();
    }
}

void DlgProject::createSkeleton(QString dir)
{
    QDir project_dir(dir);

    project_dir.mkdir("config");

    project_dir.mkdir("initial_dump");
    project_dir.mkdir("query_models");
    project_dir.mkdir("query_utils");

    project_dir.mkpath("scripts/development/done");
    project_dir.mkpath("scripts/development/error");
    project_dir.mkpath("scripts/development/review");
    project_dir.mkpath("scripts/development/run");

    project_dir.mkpath("scripts/staging/done");
    project_dir.mkpath("scripts/staging/error");
    project_dir.mkpath("scripts/staging/review");
    project_dir.mkpath("scripts/staging/run");

    project_dir.mkpath("scripts/production/done");
    project_dir.mkpath("scripts/production/error");
    project_dir.mkpath("scripts/production/review");
    project_dir.mkpath("scripts/production/run");

    project_dir.mkdir("config");
}

void DlgProject::loadConnections(QComboBox *combo)
{
    connections.sortByName();
    combo->clear();
    combo->addItem("");
    for (int i = 0; i < connections.getConnections().count(); i++) {
        ConnectionElement *conn = connections.getConnections().at(i);
        combo->addItem(conn->name());
    }
}

void DlgProject::on_bt_project_path_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (dir != "") {
        ui->ed_project_path->setText(dir);
        ui->ed_query_dir->setText(dir+"/query_utils");
        ui->ed_models_dir->setText(dir+"/query_models");
    }
}
