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
