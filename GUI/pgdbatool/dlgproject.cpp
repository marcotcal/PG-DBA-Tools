#include "dlgproject.h"
#include "ui_dlgproject.h"
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QMessageBox>

DlgProject::DlgProject(ProjectData &project, ConnectionsData &conn, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgProject),
    connections(conn),
    project(project)
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
        ui->ed_query_dir->setText(project.getQueryPath());
        ui->ed_models_dir->setText(project.getModelPath());
        ui->ed_description->setPlainText(project.getDescription());
        index = ui->cb_development_connection->findText(project.getDevelopment());
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
    project.setQueryPath(ui->ed_query_dir->text());
    project.setModelPath(ui->ed_models_dir->text());
    project.setDescription(ui->ed_description->toPlainText());

    project.writeConfig();
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
