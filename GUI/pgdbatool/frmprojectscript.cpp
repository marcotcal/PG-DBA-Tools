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
    loadRunFiles();
    loadReviewFiles();
    loadDoneFiles();
    loadErrorFiles();
}

FrmProjectScript::~FrmProjectScript()
{
    delete ui;
}

void FrmProjectScript::loadRunFiles()
{
    QString phase;
    QString file;

    switch(ui->tabWidget->currentIndex()) {
    case 0:
       phase = "development";
       break;
    case 1:
       phase = "stage";
       break;
    case 2:
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

}

void FrmProjectScript::loadReviewFiles()
{
    QString phase;
    QString file;

    switch(ui->tabWidget->currentIndex()) {
    case 0:
       phase = "development";
       break;
    case 1:
       phase = "stage";
       break;
    case 2:
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
}

void FrmProjectScript::loadErrorFiles()
{
    QString phase;
    QString file;

    switch(ui->tabWidget->currentIndex()) {
    case 0:
       phase = "development";
       break;
    case 1:
       phase = "stage";
       break;
    case 2:
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
}

void FrmProjectScript::loadDoneFiles()
{
    QString done_dir;
    QString phase;
    QString file;

    switch(ui->tabWidget->currentIndex()) {
    case 0:
       phase = "development";
       break;
    case 1:
       phase = "stage";
       break;
    case 2:
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
}

void FrmProjectScript::on_tabWidget_currentChanged(int index)
{
    loadRunFiles();
    loadReviewFiles();
    loadDoneFiles();
    loadErrorFiles();
}

void FrmProjectScript::on_bt_run_run_clicked()
{
    QString file_name = run_dir + ui->run_list->item(ui->run_list->currentRow())->text();

}

void FrmProjectScript::on_bt_run_open_clicked()
{
    QString file_name = run_dir + ui->run_list->item(ui->run_list->currentRow())->text();
    emit openFileOnSQLTool(file_name);
}

void FrmProjectScript::on_bt_review_run_clicked()
{
    QString file_name = review_dir + ui->review_list->item(ui->run_list->currentRow())->text();

}

void FrmProjectScript::on_bt_review_open_clicked()
{
    QString file_name = review_dir + ui->review_list->item(ui->run_list->currentRow())->text();
    emit openFileOnSQLTool(file_name);
}

void FrmProjectScript::on_bt_show_error_clicked()
{
    QString file_name = error_dir + ui->error_list->item(ui->run_list->currentRow())->text();

}

bool FrmProjectScript::executeScript()
{

}
