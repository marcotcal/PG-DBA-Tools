#include "frmprojectscript.h"
#include "ui_frmprojectscript.h"
#include <QDir>

FrmProjectScript::FrmProjectScript(ProjectData &project, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmProjectScript),
    project(project)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
}

FrmProjectScript::~FrmProjectScript()
{
    delete ui;
}

void FrmProjectScript::loadRunFiles()
{
    QString run_dir;
    QString phase;
    QStringList list;

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
    QDir directory(run_dir);

    ui->run_list->clear();



}

void FrmProjectScript::loadReviewFiles()
{
    QString review_dir;
    QString phase;

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


}

void FrmProjectScript::loadErrorFiles()
{
    QString error_dir;
    QString phase;

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


}

void FrmProjectScript::loadDoneFiles()
{
    QString done_dir;
    QString phase;

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


}
