#include "dlgparametersschema.h"
#include "ui_dlgparametersschema.h"

DlgParametersSchema::DlgParametersSchema(PGconn *connection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParametersSchema),
    connection(connection)
{
    ui->setupUi(this);
}

DlgParametersSchema::~DlgParametersSchema()
{
    delete ui;
}

QString DlgParametersSchema::schemaName()
{
    return ui->schema_name->currentText();
}

QString DlgParametersSchema::schemaOwner()
{
    return ui->schema_owner->currentText();
}

void DlgParametersSchema::setUserList(QStringList values)
{
    ui->schema_owner->addItems(values);
}

void DlgParametersSchema::setSchemas(QStringList values)
{
     ui->schema_name->addItems(values);
}
