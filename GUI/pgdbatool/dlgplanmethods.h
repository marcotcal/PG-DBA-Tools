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
    explicit DlgPlanMethods(QWidget *parent = nullptr);
    ~DlgPlanMethods();

private:
    Ui::DlgPlanMethods *ui;
};

#endif // DLGPLANMETHODS_H
