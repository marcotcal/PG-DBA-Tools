#include "dlgconfiguration.h"
#include "ui_dlgconfiguration.h"
#include <QFileDialog>

DlgConfiguration::DlgConfiguration(bool ready_only, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConfiguration)
{
    ui->setupUi(this);

    if (ready_only) {
        ui->path_to_sql->setEnabled(false);
        ui->path_to_query_models->setEnabled(false);
        ui->bt_open_path_sql->setEnabled(false);
        ui->bt_open_path_models->setEnabled(false);
    }
}

DlgConfiguration::~DlgConfiguration()
{
    delete ui;
}

QString DlgConfiguration::getPathToModels()
{
    return ui->path_to_query_models->text();
}

QString DlgConfiguration::getPathToSql()
{
    return ui->path_to_sql->text();
}

void DlgConfiguration::setPathToModels(QString value)
{
    ui->path_to_query_models->setText(value);
}

void DlgConfiguration::setPathToSql(QString value)
{
    ui->path_to_sql->setText(value);
}

void DlgConfiguration::on_bt_open_path_models_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (dir != "")
        ui->path_to_query_models->setText(dir);
}

void DlgConfiguration::on_bt_open_path_sql_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (dir != "")
        ui->path_to_sql->setText(dir);
}
