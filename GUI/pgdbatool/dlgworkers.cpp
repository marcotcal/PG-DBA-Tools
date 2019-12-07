#include "dlgworkers.h"
#include "ui_dlgworkers.h"

DlgWorkers::DlgWorkers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgWorkers)
{
    ui->setupUi(this);
}

DlgWorkers::~DlgWorkers()
{
    delete ui;
}

