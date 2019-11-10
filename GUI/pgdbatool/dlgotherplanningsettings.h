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

private:
    Ui::DlgOtherPlanningSettings *ui;
};

#endif // DLGOTHERPLANNINGSETTINGS_H
