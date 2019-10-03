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
    QTableWidgetItem *item;
    ui->parameter_table->clear();
    if(data->getParameters().count() >0) {
        ui->parameter_stack->setCurrentIndex(0);
        ui->parameter_table->setRowCount(data->getParameters().count());
        ui->parameter_table->setColumnCount(2);
        ui->parameter_table->setHorizontalHeaderLabels(QStringList() << "Parameter" << "Value");
        ui->parameter_table->setColumnWidth(0, 150);
        ui->parameter_table->setColumnWidth(1, 440);

        ui->parameter_table->verticalHeader()->hide();
        ui->parameter_table->horizontalHeader()->hide();

        for (int i = 0; i < data->getParameters().count(); i++) {
            item = new QTableWidgetItem(data->getParameters().at(i)->getDescription());
            item->setFlags(item->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            item->setBackgroundColor(this->palette().color(QPalette::Button));
            item->setTextColor(this->palette().color(QPalette::ButtonText));
            ui->parameter_table->setItem(i, 0, item);
            ui->parameter_table->setItem(i, 1, new QTableWidgetItem(""));
        }
        ui->parameter_table->setCurrentCell(0, 1);

        for(int i = 0; i < data->getOrders().count(); i++)
            ui->cb_order->addItem(data->getOrders().at(i)->getDescription());
    }
}

void DlgParameters::assignValues()
{
    for (int i; i < ui->parameter_table->rowCount(); i++) {
        if (ui->parameter_table->item(i, 1)->text() != "") {
            data->getParameters().at(i)->setValue(ui->parameter_table->item(i, 1)->text());
        } else {
            data->getParameters().at(i)->setValue(QVariant());
        }
    }
}

void DlgParameters::on_buttonBox_accepted()
{
    assignValues();
}
