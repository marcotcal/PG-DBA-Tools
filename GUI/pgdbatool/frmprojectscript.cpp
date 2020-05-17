#include "frmprojectscript.h"
#include "ui_frmprojectscript.h"
#include <QDir>
#include "sqltool.h"

FrmProjectScript::FrmProjectScript(ProjectData &project, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmProjectScript),
    project(project)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    mode = SqlTool::MODE_INTERNAL_DEVELOPMENT;
    loadRunFiles();
    loadReviewFiles();
    loadDoneFiles();
    loadErrorFiles();
}

FrmProjectScript::~FrmProjectScript()
{
    delete ui;
}

void FrmProjectScript::refreshAll()
{
    loadDoneFiles();
    loadErrorFiles();
    loadReviewFiles();
    loadRunFiles();
}

void FrmProjectScript::loadRunFiles()
{
    QString phase;
    QString file;

    switch(mode) {
    case SqlTool::MODE_INTERNAL_DEVELOPMENT:
       phase = "development";
       break;
    case SqlTool::MODE_INTERNAL_STAGING:
       phase = "staging";
       break;
    case SqlTool::MODE_INTERNAL_PRODUCTION:
       phase = "production";
       break;
    }

    run_dir = project.getProjectPath() + "/scripts/" + phase + "/run/";

    ui->run_list->clear();

    QDirIterator it(run_dir, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        file = it.next();
        if (QFileInfo(file).isFile()) {
            ui->run_list->addItem(QFileInfo(file).fileName());
        }
    }
    if (ui->run_list->count() > 0)
        ui->run_list->setCurrentRow(0);
}

void FrmProjectScript::loadReviewFiles()
{
    QString phase;
    QString file;

    switch(mode) {
    case SqlTool::MODE_INTERNAL_DEVELOPMENT:
       phase = "development";
       break;
    case SqlTool::MODE_INTERNAL_STAGING:
       phase = "staging";
       break;
    case SqlTool::MODE_INTERNAL_PRODUCTION:
       phase = "production";
       break;
    }

    review_dir = project.getProjectPath() + "/scripts/" + phase + "/review/";

    ui->review_list->clear();

    QDirIterator it(review_dir, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        file = it.next();
        if (QFileInfo(file).isFile()) {
            ui->review_list->addItem(QFileInfo(file).fileName());
        }
    }
    if (ui->review_list->count() > 0)
        ui->review_list->setCurrentRow(0);
}

void FrmProjectScript::loadErrorFiles()
{
    QString phase;
    QString file;

    switch(mode) {
    case SqlTool::MODE_INTERNAL_DEVELOPMENT:
       phase = "development";
       break;
    case SqlTool::MODE_INTERNAL_STAGING:
       phase = "staging";
       break;
    case SqlTool::MODE_INTERNAL_PRODUCTION:
       phase = "production";
       break;
    }

    error_dir = project.getProjectPath() + "/scripts/" + phase + "/error/";

    ui->error_list->clear();

    QDirIterator it(error_dir, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        file = it.next();
        if (QFileInfo(file).isFile()) {
            ui->error_list->addItem(QFileInfo(file).fileName());
        }
    }
    if (ui->error_list->count() > 0)
        ui->error_list->setCurrentRow(0);
}

void FrmProjectScript::loadDoneFiles()
{
    QString done_dir;
    QString phase;
    QString file;

    switch(mode) {
    case SqlTool::MODE_INTERNAL_DEVELOPMENT:
       phase = "development";
       break;
    case SqlTool::MODE_INTERNAL_STAGING:
       phase = "staging";
       break;
    case SqlTool::MODE_INTERNAL_PRODUCTION:
       phase = "production";
       break;
    }

    done_dir = project.getProjectPath() + "/scripts/" + phase + "/done/";

    ui->done_list->clear();

    QDirIterator it(done_dir, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        file = it.next();
        if (QFileInfo(file).isFile()) {
            ui->done_list->addItem(QFileInfo(file).fileName());
        }
    }
    if (ui->done_list->count() > 0)
        ui->done_list->setCurrentRow(0);
}

void FrmProjectScript::on_tabWidget_currentChanged(int index)
{
    switch(index) {
    case 0:
       mode = SqlTool::MODE_INTERNAL_DEVELOPMENT;
       break;
    case 1:
       mode = SqlTool::MODE_INTERNAL_STAGING;
       break;
    case 2:
       mode = SqlTool::MODE_INTERNAL_PRODUCTION;
       break;
    }
    loadRunFiles();
    loadReviewFiles();
    loadDoneFiles();
    loadErrorFiles();
}

void FrmProjectScript::on_bt_run_run_clicked()
{
    QString file_name = run_dir + ui->run_list->item(ui->run_list->currentRow())->text();
    emit openFileOnSQLTool(file_name, mode);
}

void FrmProjectScript::on_bt_run_open_clicked()
{
    QString file_name = run_dir + ui->run_list->item(ui->run_list->currentRow())->text();
    emit openFileOnSQLTool(file_name, SqlTool::MODE_QUERY);
}

void FrmProjectScript::on_bt_review_run_clicked()
{
    QString file_name = review_dir + ui->review_list->item(ui->review_list->currentRow())->text();
    emit openFileOnSQLTool(file_name, mode);
}

void FrmProjectScript::on_bt_review_open_clicked()
{
    QString file_name = review_dir + ui->review_list->item(ui->review_list->currentRow())->text();
    emit openFileOnSQLTool(file_name, SqlTool::MODE_QUERY);
}

void FrmProjectScript::on_bt_show_error_clicked()
{
    QString file_name = error_dir + ui->error_list->item(ui->error_list->currentRow())->text();

}

bool FrmProjectScript::executeScript()
{

}
