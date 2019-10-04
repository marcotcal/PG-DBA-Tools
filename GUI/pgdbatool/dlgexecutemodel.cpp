#include "dlgexecutemodel.h"
#include "ui_dlgexecutemodel.h"

DlgExecuteModel::DlgExecuteModel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgExecuteModel)
{
    ui->setupUi(this);
}

DlgExecuteModel::~DlgExecuteModel()
{
    delete ui;
}
