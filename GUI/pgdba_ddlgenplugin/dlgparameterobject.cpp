#include "dlgparameterobject.h"
#include "ui_dlgparameterobject.h"

DlgParameterObject::DlgParameterObject(PGconn *connection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParameterObject),
    connection(connection)
{
    ui->setupUi(this);
}

DlgParameterObject::~DlgParameterObject()
{
    delete ui;
}

QString DlgParameterObject::schemaName()
{
    return ui->schema_name->currentText();
}

QString DlgParameterObject::objectOwner()
{
    return ui->object_owner->currentText();
}

void DlgParameterObject::setLabel(QString value)
{
    ui->object_label->setText(value);
}

void DlgParameterObject::setUserList(QStringList values)
{
    ui->schema_name->addItems(values);
}

void DlgParameterObject::setSchemas(QStringList values)
{
    ui->object_owner->addItems(values);
}

void DlgParameterObject::setObjecs(QStringList values)
{
    ui->object_name->addItems(values);
}
