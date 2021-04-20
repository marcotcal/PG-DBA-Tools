#include "dlgupdate.h"
#include "ui_dlgupdate.h"

DlgUpdate::DlgUpdate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgUpdate)
{
    ui->setupUi(this);
}

DlgUpdate::~DlgUpdate()
{
    delete ui;
}

bool DlgUpdate::getAddComments()
{
    return ui->add_comments->isChecked();
}

bool DlgUpdate::getFieldTypes()
{
    return ui->show_types->isChecked();
}

void DlgUpdate::on_add_comments_toggled(bool checked)
{
    ui->show_types->setEnabled(checked);
    ui->show_types->setChecked(checked);
}
