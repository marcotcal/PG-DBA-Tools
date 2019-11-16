#ifndef DLGPLANMETHODS_H
#define DLGPLANMETHODS_H

#include <QDialog>
#include "connectionsettings.h"

namespace Ui {
class DlgPlanMethods;
}

class DlgPlanMethods : public QDialog
{
    Q_OBJECT

public:

    explicit DlgPlanMethods(ConnectionSettings *conn_settings, QWidget *parent = nullptr);
    ~DlgPlanMethods();

private slots:
    void on_cb_unit_currentIndexChanged(int index);
    void on_buttonBox_accepted();
    void on_sp_efective_cache_size_editingFinished();

private:
    Ui::DlgPlanMethods *ui;
    ConnectionSettings *conn_settings;
    void initialize();

};

#endif // DLGPLANMETHODS_H
