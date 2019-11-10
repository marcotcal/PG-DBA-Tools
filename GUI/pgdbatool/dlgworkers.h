#ifndef DLGWORKERS_H
#define DLGWORKERS_H

#include <QDialog>

namespace Ui {
class DlgWorkers;
}

class DlgWorkers : public QDialog
{
    Q_OBJECT

public:
    explicit DlgWorkers(QWidget *parent = nullptr);
    ~DlgWorkers();

    void setWorkMemory(int value);
    void setMaintenanceWorkMemory(int value);
    void setEfectiveIOConcurrency(int value);
    void setMaxWorkerProcess(int value);
    void setMaxParallelWorkersPerGather(int value);
    void setParallelWorkers(int value);

    int getWorkMemory();
    int getMaintenanceWorkMemory();
    int getEfectiveIOConcurrency();
    int getMaxWorkerProcess();
    int getMaxParallelWorkersPerGather();
    int getParallelWorkers();

private:
    Ui::DlgWorkers *ui;
};

#endif // DLGWORKERS_H
