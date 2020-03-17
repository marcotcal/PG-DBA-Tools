#include "frmfindtext.h"
#include "ui_frmfindtext.h"

FrmFindText::FrmFindText(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmFindText)
{
    ui->setupUi(this);
}

FrmFindText::~FrmFindText()
{
    delete ui;
}

void FrmFindText::on_bt_find_clicked()
{

}

void FrmFindText::on_bt_close_clicked()
{
    close();
}
