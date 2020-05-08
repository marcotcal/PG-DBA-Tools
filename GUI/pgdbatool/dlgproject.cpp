#include "dlgproject.h"
#include "ui_dlgproject.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QMessageBox>
#include "dlgconnections.h"

DlgProject::DlgProject(ProjectData &project, ConnectionsData &conn, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgProject),
    connections(conn),
    project(project)
{
    ui->setupUi(this);

    loadCombo(ui->cb_development_connection);
    loadCombo(ui->cb_staging_connection);
    loadCombo(ui->cb_production_connection);
}

DlgProject::~DlgProject()
{
    delete ui;
}

int DlgProject::projectAdd()
{
    return exec();
}

int DlgProject::projectEdit()
{
    int index;

    if (project.getProjectName() != "") {

        ui->ed_project_name->setText(project.getProjectName());
        ui->ed_project_path->setText(project.getProjectPath());
        ui->ed_project_path->setEnabled(false);
        ui->bt_project_path->setEnabled(false);
        ui->ed_query_dir->setText(project.getQueryPath());
        ui->ed_models_dir->setText(project.getModelPath());
        ui->ed_description->setPlainText(project.getDescription());
        index = ui->cb_development_connection->findText(project.getDevelopment());
        if ( index != 0 ) {
           ui->cb_development_connection->setCurrentIndex(index);
           loadDBCombo(ui->cb_development_database, index-1);
        }
        index = ui->cb_staging_connection->findText(project.getStaging());
        if ( index != 0 ) {
           ui->cb_staging_connection->setCurrentIndex(index);
           loadDBCombo(ui->cb_staging_database, index-1);
        }
        index = ui->cb_production_connection->findText(project.getProduction());
        if ( index != 0 ) {
           ui->cb_production_connection->setCurrentIndex(index);
           loadDBCombo(ui->cb_production_database, index-1);
        }
        index = ui->cb_development_database->findText(project.getDevelopmentDatabase());
        if ( index != 0 ) {
           ui->cb_development_database->setCurrentIndex(index);
        }
        index = ui->cb_staging_database->findText(project.getStagingDatabase());
        if ( index != 0 ) {
           ui->cb_staging_database->setCurrentIndex(index);
        }
        index = ui->cb_production_database->findText(project.getProductionDatabase());
        if ( index != 0 ) {
           ui->cb_production_database->setCurrentIndex(index);
        }
        return exec();

    } else {
        return QDialog::Rejected;
    }

}

void DlgProject::createProject()
{
    project.setProjectPath(ui->ed_project_path->text());
    project.setProjectName(ui->ed_project_name->text());
    project.setDevelopment(ui->cb_development_connection->itemText(
                               ui->cb_development_connection->currentIndex()));
    project.setStaging(ui->cb_staging_connection->itemText(
                           ui->cb_staging_connection->currentIndex()));
    project.setProduction(ui->cb_production_connection->itemText(
                              ui->cb_production_connection->currentIndex()));
    project.setDevelopmentDatabase(ui->cb_development_database->itemText(
                               ui->cb_development_database->currentIndex()));
    project.setStagingDatabase(ui->cb_staging_database->itemText(
                           ui->cb_staging_database->currentIndex()));
    project.setProductionDatabase(ui->cb_production_database->itemText(
                              ui->cb_production_database->currentIndex()));
    project.setQueryPath(ui->ed_query_dir->text());
    project.setModelPath(ui->ed_models_dir->text());
    project.setDescription(ui->ed_description->toPlainText());

}

void DlgProject::createSkeleton()
{
    QDir project_dir(ui->ed_project_path->text());

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

void DlgProject::loadCombo(QComboBox *combo)
{
    project.getConnnections().sortByName();
    combo->clear();
    combo->addItem("");
    for (int i = 0; i < project.getConnnections().getConnections().count(); i++) {
        ConnectionElement *conn = project.getConnnections().getConnections().at(i);
        combo->addItem(conn->name());
    }
}

void DlgProject::loadDBCombo(QComboBox *combo, int connection)
{
    QStringList databases;

    combo->clear();

    databases = connections.getConnections().at(connection)->getDatabaseList();
    for (int i = 0; i < databases.count(); i++) {
        combo->addItem(databases.at(i));
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

void DlgProject::done(int res)
{
    QString pdir = ui->ed_project_path->text();
    QString qdir = ui->ed_query_dir->text();
    QString mdir = ui->ed_models_dir->text();
    QDir project_dir(pdir);
    QDir query_dir(qdir);
    QDir model_dir(mdir);
    QMessageBox msgBox;

    if (res == QDialog::Accepted)
    {
        if (pdir.trimmed() == "") {
            msgBox.setText("The project path must be informed.");
            msgBox.exec();
            ui->ed_project_path->setFocus();
            return;
        }

        if (!project_dir.exists()) {
            msgBox.setText("The project path does not exists.");
            msgBox.exec();
            ui->ed_project_path->setFocus();
            return;
        }

        // create skeleton
        createSkeleton();

        if (qdir.trimmed() == "") {
            msgBox.setText("The query path must be informed.");
            msgBox.exec();
            ui->ed_query_dir->setFocus();
            return;
        }

        if (!query_dir.exists()) {
            msgBox.setText("The query path does not exists.");
            msgBox.exec();
            ui->ed_query_dir->setFocus();
            return;
        }

        if (mdir.trimmed() == "") {
            msgBox.setText("The models path must be informed.");
            msgBox.exec();
            ui->ed_models_dir->setFocus();
            return;
        }

        if (!model_dir.exists()) {
            msgBox.setText("The models path does not exists.");
            msgBox.exec();
            ui->ed_models_dir->setFocus();
            return;
        }

        if (ui->ed_project_name->text() == "") {
            msgBox.setText("The project name must be informed.");
            msgBox.exec();
            ui->ed_project_name->setFocus();
            return;
        }

        createProject();

    }

    QDialog::done(res);
}

void DlgProject::on_bt_connections_clicked()
{
    int index = -1;
    DlgConnections dlg(project.getConnnections());
    dlg.exec();
    loadCombo(ui->cb_development_connection);
    loadCombo(ui->cb_staging_connection);
    loadCombo(ui->cb_production_connection);
    if ( index != -1 ) {
       ui->cb_development_connection->setCurrentIndex(index);
    }
    index = ui->cb_staging_connection->findText(project.getStaging());
    if ( index != -1 ) {
       ui->cb_staging_connection->setCurrentIndex(index);
    }
    index = ui->cb_production_connection->findText(project.getProduction());
    if ( index != -1 ) {
       ui->cb_production_connection->setCurrentIndex(index);
    }
}

void DlgProject::on_cb_development_connection_activated(int index)
{
    index--;
    if (index  == -1) {
        ui->cb_development_database->clear();
        return;
    }
    loadDBCombo(ui->cb_development_database, index);
}
void DlgProject::on_cb_staging_connection_activated(int index)
{
    index--;
    if (index  == -1) {
        ui->cb_staging_database->clear();
        return;
    }
    loadDBCombo(ui->cb_staging_database, index);
}

void DlgProject::on_cb_production_connection_activated(int index)
{
    index--;
    if (index  == -1) {
        ui->cb_production_database->clear();
        return;
    }
    loadDBCombo(ui->cb_production_database, index);
}
