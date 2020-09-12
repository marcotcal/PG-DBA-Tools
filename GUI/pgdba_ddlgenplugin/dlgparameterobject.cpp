#include "dlgparameterobject.h"
#include "ui_dlgparameterobject.h"

DlgParameterObject::DlgParameterObject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParameterObject)
{
    ui->setupUi(this);
}

DlgParameterObject::~DlgParameterObject()
{
    delete ui;
}
