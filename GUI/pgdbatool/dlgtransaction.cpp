#include "dlgtransaction.h"
#include "ui_dlgtransaction.h"

DlgTransaction::DlgTransaction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgTransaction)
{
    ui->setupUi(this);
}

DlgTransaction::~DlgTransaction()
{
    delete ui;
}

void DlgTransaction::on_buttonBox_accepted()
{
    command = "START TRANSACTION ISOLATION LEVEL ";

    if (ui->serializable->isChecked())
        command += "SERIALIZABLE ";
    else if (ui->repeatable_read->isChecked())
        command += "REPEATABLE READ ";
    else if (ui->read_commited->isChecked())
        command += "READ COMMITTED ";
    else if (ui->read_uncommited->isChecked())
        command += "READ UNCOMMITTED ";

    if (ui->read_write->isChecked())
        command += "READ WRITE ";
    else if (ui->read_only->isChecked())
        command += "READ ONLY ";

    if (ui->deferrable->isChecked())
        command += "DEFERRABLE ";
    else
        command += "NOT DEFERRABLE ";
}
