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

void DlgGQOConfig::setEnableGEQO(bool enable)
{
    ui->ck_enable_geqo->setChecked(enable);
}

void DlgGQOConfig::setThreshold(int value)
{
    ui->sp_geqo_threshold->setValue(value);
}

void DlgGQOConfig::setEffort(int value)
{
    ui->sp_geqo_effort->setValue(value);
}

void DlgGQOConfig::setPoolSize(int value)
{
    ui->sp_geqo_pool_size->setValue(value);
}

void DlgGQOConfig::setGenerations(int value)
{
    ui->sp_geqo_generations->setValue(value);
}

void DlgGQOConfig::setSelectionBias(double value)
{
    ui->sp_geqo_selection_bias->setValue(value);
}

void DlgGQOConfig::setSeed(double seed)
{
    ui->sp_geqo_seed->setValue(seed);
}

bool DlgGQOConfig::getEnableGEQO()
{
    return ui->ck_enable_geqo->isChecked();
}

int DlgGQOConfig::getThreshold()
{
    return ui->sp_geqo_threshold->value();
}

int DlgGQOConfig::getEffort()
{
    return ui->sp_geqo_effort->value();
}

int DlgGQOConfig::getPoolSize()
{
    return ui->sp_geqo_pool_size->value();
}

int DlgGQOConfig::getGenerations()
{
    return ui->sp_geqo_generations->value();
}

double DlgGQOConfig::getSelectionBias()
{
    return ui->sp_geqo_selection_bias->value();
}

double DlgGQOConfig::getSeed()
{
    return ui->sp_geqo_seed->value();
}
