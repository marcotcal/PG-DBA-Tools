#include "dlgproject.h"
#include "ui_dlgproject.h"

DlgProject::DlgProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgProject)
{
    ui->setupUi(this);
}

DlgProject::~DlgProject()
{
    delete ui;
}
