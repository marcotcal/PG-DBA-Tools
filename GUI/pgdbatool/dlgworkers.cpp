#include "dlgworkers.h"
#include "ui_dlgworkers.h"

DlgWorkers::DlgWorkers(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgWorkers)
{
    ui->setupUi(this);
}

DlgWorkers::~DlgWorkers()
{
    delete ui;
}

void DlgWorkers::setWorkMemory(int value)
{
    ui->sp_work_mem->setValue(value);
}

void DlgWorkers::setMaintenanceWorkMemory(int value)
{
    ui->sp_maintenance_work_mem->setValue(value);
}

void DlgWorkers::setEfectiveIOConcurrency(int value)
{
    ui->sp_efective_io_concurrency->setValue(value);
}

void DlgWorkers::setMaxWorkerProcess(int value)
{
    ui->sp_max_work_process->setValue(value);
}

void DlgWorkers::setMaxParallelWorkersPerGather(int value)
{
    ui->sp_max_parallel_workers_per_gather->setValue(value);
}

void DlgWorkers::setParallelWorkers(int value)
{
    ui->sp_parallel_workers->setValue(value);
}

int DlgWorkers::getWorkMemory()
{
    return ui->sp_work_mem->value();
}

int DlgWorkers::getMaintenanceWorkMemory()
{
    return ui->sp_maintenance_work_mem->value();
}

int DlgWorkers::getEfectiveIOConcurrency()
{
    return ui->sp_efective_io_concurrency->value();
}

int DlgWorkers::getMaxWorkerProcess()
{
    return ui->sp_max_work_process->value();
}

int DlgWorkers::getMaxParallelWorkersPerGather()
{
    return ui->sp_max_parallel_workers_per_gather->value();
}

int DlgWorkers::getParallelWorkers()
{
    return ui->sp_parallel_workers->value();
}

