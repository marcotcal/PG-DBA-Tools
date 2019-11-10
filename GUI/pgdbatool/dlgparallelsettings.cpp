#include "dlgparallelsettings.h"
#include "ui_dlgparallelsettings.h"

DlgParallelSettings::DlgParallelSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParallelSettings)
{
    ui->setupUi(this);
}

DlgParallelSettings::~DlgParallelSettings()
{
    delete ui;
}

void DlgParallelSettings::setForceParallelMode(bool force)
{
    ui->ck_force_parallel_mode->setChecked(force);
}

void DlgParallelSettings::setParallelSetupCost(double cost)
{
    ui->sp_parallel_setup_cost->setValue(cost);
}

void DlgParallelSettings::setParallelTupleCost(double cost)
{
    ui->sp_parallel_tuple_cost->setValue(cost);
}

void DlgParallelSettings::setMinimumParallelTableScanSize(int size)
{
    ui->sp_min_parallel_scan_size->setValue(size);
}

void DlgParallelSettings::setParallelIndexScanSize(int size)
{
    ui->sp_min_parallel_index_scan_size->setValue(size);
}

bool DlgParallelSettings::getForceParallelMode()
{
    return ui->ck_force_parallel_mode->isChecked();
}

double DlgParallelSettings::getParallelSetupCost()
{
    return ui->sp_parallel_setup_cost->value();
}

double DlgParallelSettings::getParallelTupleCost()
{
    ui->sp_parallel_tuple_cost->value();
}

int DlgParallelSettings::getMinimumParallelTableScanSize()
{
    ui->sp_min_parallel_scan_size->value();
}

int DlgParallelSettings::getParallelIndexScanSize()
{
    ui->sp_min_parallel_index_scan_size->value();
}
