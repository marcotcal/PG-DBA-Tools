#include "dlgparameters.h"
#include "ui_dlgparameters.h"

DlgParameters::DlgParameters(QueryModelData *data, ConnectionsData &connections,
                             int sel_connection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParameters),
    connections(connections),
    sel_connection(sel_connection),
    model_data(data)
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
    if(model_data->getParameters().count() >0) {
        ui->parameter_stack->setCurrentIndex(0);
        ui->parameter_table->setRowCount(model_data->getParameters().count());
        ui->parameter_table->setColumnCount(2);
        ui->parameter_table->setHorizontalHeaderLabels(QStringList() << "Parameter" << "Value");
        ui->parameter_table->setColumnWidth(0, 150);
        ui->parameter_table->setColumnWidth(1, 440);

        ui->parameter_table->verticalHeader()->hide();
        ui->parameter_table->horizontalHeader()->hide();

        for (int i = 0; i < model_data->getParameters().count(); i++) {
            item = new QTableWidgetItem(model_data->getParameters().at(i)->getDescription());
            item->setFlags(item->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            item->setBackgroundColor(this->palette().color(QPalette::Button));
            item->setTextColor(this->palette().color(QPalette::ButtonText));
            ui->parameter_table->setItem(i, 0, item);
            ui->parameter_table->setItem(i, 1, new QTableWidgetItem(""));
        }
        ui->parameter_table->setCurrentCell(0, 1);        
    }
    for(int i = 0; i < model_data->getOrders().count(); i++)
        ui->cb_order->addItem(model_data->getOrders().at(i)->getDescription());
}

void DlgParameters::assignValues()
{
    for (int i = 0; i < ui->parameter_table->rowCount(); i++) {
        if (ui->parameter_table->item(i, 1)->text() != "") {
            model_data->getParameters().at(i)->setValue(ui->parameter_table->item(i, 1)->text());
        } else {
            model_data->getParameters().at(i)->setValue(QVariant());
        }
    }
    if (model_data->getOrders().count() > 0)
        model_data->setOrderBy(" ORDER BY " + model_data->getOrders().at(ui->cb_order->currentIndex())->getFields());
    else
        model_data->setOrderBy("");
}

void DlgParameters::on_buttonBox_accepted()
{
    assignValues();
}
