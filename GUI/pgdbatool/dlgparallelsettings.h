#ifndef DLGPARALLELSETTINGS_H
#define DLGPARALLELSETTINGS_H

#include <QDialog>

namespace Ui {
class DlgParallelSettings;
}

class DlgParallelSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DlgParallelSettings(QWidget *parent = nullptr);
    ~DlgParallelSettings();

    void setForceParallelMode(bool force);
    void setParallelSetupCost(double cost);
    void setParallelTupleCost(double cost);
    void setMinimumParallelTableScanSize(int size);
    void setParallelIndexScanSize(int size);

    bool getForceParallelMode();
    double getParallelSetupCost();
    double getParallelTupleCost();
    int getMinimumParallelTableScanSize();
    int getParallelIndexScanSize();

private:
    Ui::DlgParallelSettings *ui;
};

#endif // DLGPARALLELSETTINGS_H
