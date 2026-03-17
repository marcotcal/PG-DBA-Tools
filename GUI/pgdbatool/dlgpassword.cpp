#include "dlgpassword.h"
#include "ui_dlgpassword.h"

DlgPassword::DlgPassword(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgPassword)
{
    ui->setupUi(this);
}

DlgPassword::~DlgPassword()
{
    delete ui;
}

QString DlgPassword::getPassword()
{
    return ui->textEdit->toPlainText();
}
