#include "dlgconnections.h"
#include "ui_dlgconnections.h"

DlgConnections::DlgConnections(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConnections)
{
    ui->setupUi(this);    
}

DlgConnections::~DlgConnections()
{
    delete ui;
}

void DlgConnections::on_bt_close_clicked()
{
    close();
}

void DlgConnections::on_bt_add_connection_clicked()
{

}

void DlgConnections::on_bt_delete_connection_clicked()
{

}
