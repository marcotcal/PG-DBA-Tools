#include "dlgmenunew.h"
#include "ui_dlgmenunew.h"

DlgMenuNew::DlgMenuNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMenuNew)
{
    ui->setupUi(this);
}

DlgMenuNew::~DlgMenuNew()
{
    delete ui;
}
