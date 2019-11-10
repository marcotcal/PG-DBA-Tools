#ifndef DLGPLANMETHODS_H
#define DLGPLANMETHODS_H

#include <QDialog>

namespace Ui {
class DlgPlanMethods;
}

class DlgPlanMethods : public QDialog
{
    Q_OBJECT

public:
    enum {
        PL_BITMAP_SCAN = 1,
        PL_HASH_AGGREGATIONS = 3,
        PL_HASH_JOINS = 7,
        PL_INDEX_SCAN = 15,
        PL_INDEX_ONLY_SCAN = 31,
        PL_MATERIALIZATION = 63,
        PL_NESTED_LOOPS = 127,
        PL_SEQUENTIAL_SCAN = 255,
        PL_SORT_STEPS = 511,
        PL_TDI_SCAN = 1023
    };
    explicit DlgPlanMethods(QWidget *parent = nullptr);
    ~DlgPlanMethods();
    void setPlanFlags(int flags);
    void setSequentialPageCost(double cost);
    void setRandomPageCost(double cost);
    void setTupleCost(double cost);
    void setOperatorCost(double cost);
    void setEfectiveCashSize(int size);
    int getPlanFlags();
    double getSequetialPageCost();
    double getRandomPageCost();
    double getTupleCost();
    double getOperatorCost();
    int getEfectivenessCashSize();
private:
    Ui::DlgPlanMethods *ui;
};

#endif // DLGPLANMETHODS_H
