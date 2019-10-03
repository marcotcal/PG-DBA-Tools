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

QString DlgParameters::queryText()
{

}

void DlgParameters::loadParameters()
{
    QTableWidgetItem *item;
    ui->parameter_table->clear();
    if(data->getParameters().count() >0) {
        ui->parameter_stack->setCurrentIndex(0);
        ui->parameter_table->setRowCount(data->getParameters().count());
        ui->parameter_table->setColumnCount(2);
        ui->parameter_table->setHorizontalHeaderLabels(QStringList() << "Parameter" << "Value");
        ui->parameter_table->setColumnWidth(0, 150);
        ui->parameter_table->setColumnWidth(1, 200);

        for (int i = 0; i < data->getParameters().count(); i++) {
            item = new QTableWidgetItem(data->getParameters().at(i)->getDescription());
            ui->parameter_table->setItem(i, 0, item);
        }

    }
}
