#include "dlgparallelsettings.h"
#include "ui_dlgparallelsettings.h"
#include <QMessageBox>

DlgParallelSettings::DlgParallelSettings(ConnectionSettings *conn_settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParallelSettings),
    conn_settings(conn_settings)
{
    ui->setupUi(this);
}

DlgParallelSettings::~DlgParallelSettings()
{
    delete ui;
}

void DlgParallelSettings::initialize()
{
    QVariant value;

    value = conn_settings->getSetting("force_parallel_mode");
    if (value.isValid())
        ui->ck_force_parallel_mode->setChecked(value == "on");
    else
        ui->ck_force_parallel_mode->setEnabled(false);

    value = conn_settings->getSetting("parallel_setup_cost");
    ui->sp_parallel_setup_cost->setValue(value.toDouble());

    value = conn_settings->getSetting("parallel_tuple_cost");
    ui->sp_parallel_tuple_cost->setValue(value.toDouble());

    value = conn_settings->getSetting("min_parallel_table_scan_size");
    ui->sp_min_parallel_scan_size->setValue(value.toInt());

    value = conn_settings->getSetting("min_parallel_index_scan_size");
    ui->sp_min_parallel_index_scan_size->setValue(value.toInt());
}

void DlgParallelSettings::on_buttonBox_accepted()
{
    QMessageBox msg;

    try {
        conn_settings->alterSetting("force_parallel_mode",
                                    ui->ck_force_parallel_mode->isChecked() ? "on" : "off");

        conn_settings->alterSetting("parallel_setup_cost", ui->sp_parallel_setup_cost->value());
        conn_settings->alterSetting("parallel_tuple_cost", ui->sp_parallel_tuple_cost->value());
        conn_settings->alterSetting("min_parallel_table_scan_size", ui->sp_min_parallel_scan_size->value());
        conn_settings->alterSetting("min_parallel_index_scan_size", ui->sp_min_parallel_index_scan_size->value());

    } catch(SettingsException &e) {
        msg.setText(e.getMessage());
        msg.exec();
    }
}
