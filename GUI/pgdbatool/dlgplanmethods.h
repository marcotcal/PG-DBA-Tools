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
        PL_HASH_AGGREGATIONS = 2,
        PL_HASH_JOINS = 4,
        PL_INDEX_SCAN = 8,
        PL_INDEX_ONLY_SCAN = 16,
        PL_MATERIALIZATION = 32,
        PL_NESTED_LOOPS = 64,
        PL_SEQUENTIAL_SCAN = 128,
        PL_SORT_STEPS = 256,
        PL_TDI_SCAN = 512,
        PL_GATHER_MERGE = 1024,
        PL_MERGE_JOIN = 2048
    };
    explicit DlgPlanMethods(QWidget *parent = nullptr);
    ~DlgPlanMethods();
    void setPlanFlags(int flags);
    void setDisableFlags(int flags);
    void setSequentialPageCost(double cost);
    void setRandomPageCost(double cost);
    void setTupleCost(double cost);
    void setIndexTupleCost(double cost);
    void setOperatorCost(double cost);
    void setEfectiveCashSize(int size);
    int getPlanFlags();
    double getSequetialPageCost();
    double getRandomPageCost();
    double getTupleCost();
    double getIndexTupleCost();
    double getOperatorCost();
    int getEfectivenessCashSize();
private:
    Ui::DlgPlanMethods *ui;
};

#endif // DLGPLANMETHODS_H
