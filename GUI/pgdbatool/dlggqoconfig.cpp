#include "dlggqoconfig.h"
#include "ui_dlggqoconfig.h"

DlgGQOConfig::DlgGQOConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgGQOConfig)
{
    ui->setupUi(this);
}

DlgGQOConfig::~DlgGQOConfig()
{
    delete ui;
}
