#ifndef DLGOTHERPLANNINGSETTINGS_H
#define DLGOTHERPLANNINGSETTINGS_H

#include <QDialog>

namespace Ui {
class DlgOtherPlanningSettings;
}

class DlgOtherPlanningSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DlgOtherPlanningSettings(QWidget *parent = nullptr);
    ~DlgOtherPlanningSettings();

    void setDefaultStatisticsTarget(int value);
    void setConstraintExclusion(bool value);
    void setCursorTupleFraction(double value);
    void setFromCollapseLimit(int value);
    void setJoinCollapseLimit(int value);

    int getDefaultStatisticsTarget();
    bool getConstraintExeclusion();
    double getCursorTupleFraction();
    int getFromCollapseLimit();
    int getJoinCollapseLimit();

private:
    Ui::DlgOtherPlanningSettings *ui;
};

#endif // DLGOTHERPLANNINGSETTINGS_H
