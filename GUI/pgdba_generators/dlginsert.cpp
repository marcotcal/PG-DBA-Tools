#include "dlginsert.h"
#include "ui_dlginsert.h"

DlgInsert::DlgInsert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgInsert)
{
    ui->setupUi(this);
}

DlgInsert::~DlgInsert()
{
    delete ui;
}
