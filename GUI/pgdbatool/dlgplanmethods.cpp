#include "dlgplanmethods.h"
#include "ui_dlgplanmethods.h"

DlgPlanMethods::DlgPlanMethods(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPlanMethods)
{
    ui->setupUi(this);
}

DlgPlanMethods::~DlgPlanMethods()
{
    delete ui;
}
