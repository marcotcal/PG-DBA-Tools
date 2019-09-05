#include "dlgconfiguration.h"
#include "ui_dlgconfiguration.h"

DlgConfiguration::DlgConfiguration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConfiguration)
{
    ui->setupUi(this);
}

DlgConfiguration::~DlgConfiguration()
{
    delete ui;
}
