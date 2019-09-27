#include "dlgparameters.h"
#include "ui_dlgparameters.h"

DlgParameters::DlgParameters(QueryModelData &data, QWidget *parent) :
    QDialog(parent),
    data(data),
    ui(new Ui::DlgParameters)
{
    ui->setupUi(this);
    ui->parameter_stack->setCurrentIndex(1);
}

DlgParameters::~DlgParameters()
{
    delete ui;
}

void DlgParameters::loadParameters()
{
    if(data.getParameters().count() >0) {
        ui->parameter_stack->setCurrentIndex(1);
        ui->parameter_table->setRowCount(data.getParameters().count());
        ui->parameter_table->setColumnCount(2);
    }
}
