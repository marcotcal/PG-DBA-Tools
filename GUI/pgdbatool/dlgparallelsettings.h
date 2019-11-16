#ifndef DLGPARALLELSETTINGS_H
#define DLGPARALLELSETTINGS_H

#include <QDialog>
#include "connectionsettings.h"

namespace Ui {
class DlgParallelSettings;
}

class DlgParallelSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DlgParallelSettings(ConnectionSettings *conn_settings, QWidget *parent = nullptr);
    ~DlgParallelSettings();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DlgParallelSettings *ui;
    ConnectionSettings *conn_settings;
    void initialize();
};

#endif // DLGPARALLELSETTINGS_H
