#ifndef DLGGQOCONFIG_H
#define DLGGQOCONFIG_H

#include <QDialog>
#include "connectionsettings.h"

namespace Ui {
class DlgGQOConfig;
}

class DlgGQOConfig : public QDialog
{
    Q_OBJECT

public:
    explicit DlgGQOConfig(ConnectionSettings *conn_settings, QWidget *parent = nullptr);
    ~DlgGQOConfig();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DlgGQOConfig *ui;
    ConnectionSettings *conn_settings;
    void initialize();
};

#endif // DLGGQOCONFIG_H
