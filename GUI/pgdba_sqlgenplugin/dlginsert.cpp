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

bool DlgInsert::getAddComments()
{
    return ui->add_comments->isChecked();
}

bool DlgInsert::getFieldTypes()
{
    return ui->show_types->isChecked();
}

bool DlgInsert::getOnlyMandatory()
{
    return ui->only_mandatory->isChecked();
}

bool DlgInsert::getRemoveMandatoryWithDefaults()
{
    return ui->remove_mandatory_with_defaults->isChecked();
}

int DlgInsert::getOnConflict()
{
    if(ui->no_on_conflict_clause->isChecked()) return NO_CONFLICT_CLAUSE;
    else if(ui->do_nothing->isChecked()) return ON_CONFLICT_DO_NOTHING;
    else if(ui->update->isChecked()) return ON_CONFLICT_UPDATE;
    return -1;
}


void DlgInsert::on_add_comments_toggled(bool checked)
{
    ui->show_types->setEnabled(checked);
    ui->show_types->setChecked(checked);
}
