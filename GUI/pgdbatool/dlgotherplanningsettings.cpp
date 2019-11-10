#include "dlgotherplanningsettings.h"
#include "ui_dlgotherplanningsettings.h"

DlgOtherPlanningSettings::DlgOtherPlanningSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgOtherPlanningSettings)
{
    ui->setupUi(this);
}

DlgOtherPlanningSettings::~DlgOtherPlanningSettings()
{
    delete ui;
}

void DlgOtherPlanningSettings::setDefaultStatisticsTarget(int value)
{
    ui->sp_default_statistics_target->setValue(value);
}

void DlgOtherPlanningSettings::setConstraintExclusion(bool value)
{
    ui->ck_constraint_exclusion->setChecked(value);
}

void DlgOtherPlanningSettings::setCursorTupleFraction(double value)
{
    ui->sp_cursor_tuple_fraction->setValue(value);
}

void DlgOtherPlanningSettings::setFromCollapseLimit(int value)
{
    ui->sp_from_collapse_limit->setValue(value);
}

void DlgOtherPlanningSettings::setJoinCollapseLimit(int value)
{
    ui->sp_join_collapse_limit->setValue(value);
}

int DlgOtherPlanningSettings::getDefaultStatisticsTarget()
{
    return ui->sp_default_statistics_target->value();
}

bool DlgOtherPlanningSettings::getConstraintExeclusion()
{
    return ui->ck_constraint_exclusion->isChecked();
}

double DlgOtherPlanningSettings::getCursorTupleFraction()
{
    return ui->sp_cursor_tuple_fraction->value();
}

int DlgOtherPlanningSettings::getFromCollapseLimit()
{
    return ui->sp_from_collapse_limit->value();
}

int DlgOtherPlanningSettings::getJoinCollapseLimit()
{
    return ui->sp_join_collapse_limit->value();
}
