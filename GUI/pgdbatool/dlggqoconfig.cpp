#include "dlggqoconfig.h"
#include "ui_dlggqoconfig.h"
#include <QMessageBox>

DlgGQOConfig::DlgGQOConfig(ConnectionSettings *conn_settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgGQOConfig),
    conn_settings(conn_settings)
{
    ui->setupUi(this);
    initialize();
}

DlgGQOConfig::~DlgGQOConfig()
{
    delete ui;
}

void DlgGQOConfig::initialize()
{

    QVariant value;

    value = conn_settings->getSetting("geqo");
    if (value.isValid())
        ui->ck_enable_geqo->setChecked(value.toString() == "on");
    else
        ui->ck_enable_geqo->setEnabled(false);

    value = conn_settings->getSetting("geqo_threshold");
    ui->sp_geqo_threshold->setValue(value.toInt());

    value = conn_settings->getSetting("geqo_effort");
    ui->sp_geqo_effort->setValue(value.toInt());

    value = conn_settings->getSetting("geqo_pool_size");
    ui->sp_geqo_pool_size->setValue(value.toInt());

    value = conn_settings->getSetting("geqo_generations");
    ui->sp_geqo_generations->setValue(value.toInt());

    value = conn_settings->getSetting("geqo_selection_bias");
    ui->sp_geqo_selection_bias->setValue(value.toDouble());

    value = conn_settings->getSetting("geqo_seed");
    ui->sp_geqo_seed->setValue(value.toDouble());

}

void DlgGQOConfig::on_buttonBox_accepted()
{
    QMessageBox msg;

    try {
        conn_settings->alterSetting("geqo",
                                    ui->ck_enable_geqo->isChecked() ? "on" : "off");

        conn_settings->alterSetting("geqo_threshold", ui->sp_geqo_threshold->value());
        conn_settings->alterSetting("geqo_effort", ui->sp_geqo_effort->value());
        conn_settings->alterSetting("geqo_pool_size", ui->sp_geqo_pool_size->value());
        conn_settings->alterSetting("geqo_generations", ui->sp_geqo_generations->value());
        conn_settings->alterSetting("geqo_selection_bias", ui->sp_geqo_selection_bias->value());
        conn_settings->alterSetting("geqo_seed", ui->sp_geqo_seed->value());

    } catch(SettingsException &e) {
        msg.setText(e.getMessage());
        msg.exec();
    }
}
