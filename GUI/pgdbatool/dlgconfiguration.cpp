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
    ui->tabs->setCurrentIndex(0);
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

int DlgConfiguration::getOptions()
{
    int value = 0;

    if (ui->include_task_code->isChecked())
        value += SCG_INCLUDE_TASK_CODE;

    if (ui->include_sprint_code->isChecked())
        value += SCG_INCLUDE_SPRINT_CODE;

    if (ui->serialize_by_date->isChecked())
        value += SCG_SERIALIZE_BY_DATE;

    if (ui->serialize_by_integer->isChecked())
        value += SCG_SERIALIZE_BY_INTEGER;

    if (ui->keep_script_on_review->isChecked())
        value += SCG_KEEP_ON_REVIEW;

    return value;
}

QString DlgConfiguration::getTaskPrefix()
{
    return ui->task_code_prefix->text();
}

void DlgConfiguration::setPathToModels(QString value)
{
    ui->path_to_query_models->setText(value);
}

void DlgConfiguration::setPathToSql(QString value)
{
    ui->path_to_sql->setText(value);
}

void DlgConfiguration::setOptions(int value)
{
    ui->include_task_code->setChecked((value & SCG_INCLUDE_TASK_CODE));
    ui->include_sprint_code->setChecked((value & SCG_INCLUDE_SPRINT_CODE));
    ui->keep_script_on_review->setChecked((value & SCG_KEEP_ON_REVIEW));
    ui->serialize_by_date->setChecked((value & SCG_SERIALIZE_BY_DATE));
    ui->serialize_by_integer->setChecked((value & SCG_SERIALIZE_BY_INTEGER));
}

void DlgConfiguration::setTaskPrefix(QString value)
{
    ui->task_code_prefix->setText(value);
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
