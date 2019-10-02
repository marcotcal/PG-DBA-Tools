#include "dlgparameters.h"
#include "ui_dlgparameters.h"

DlgParameters::DlgParameters(QueryModelData *data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParameters),
    data(data)
{
    ui->setupUi(this);
    ui->parameter_stack->setCurrentIndex(1);
    loadParameters();
}

DlgParameters::~DlgParameters()
{
    delete ui;
}

void DlgParameters::loadParameters()
{
    ui->parameter_table->clear();
    if(data->getParameters().count() >0) {
        ui->parameter_stack->setCurrentIndex(0);
        ui->parameter_table->setRowCount(data->getParameters().count());
        ui->parameter_table->setColumnCount(2);
    }
}
