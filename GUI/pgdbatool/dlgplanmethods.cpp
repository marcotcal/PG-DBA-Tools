#include "dlgplanmethods.h"
#include "ui_dlgplanmethods.h"
#include <QMessageBox>

DlgPlanMethods::DlgPlanMethods(ConnectionSettings *conn_settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPlanMethods),
    conn_settings(conn_settings)
{
    ui->setupUi(this);
    initialize();
}

DlgPlanMethods::~DlgPlanMethods()
{
    delete ui;
}

void DlgPlanMethods::initialize()
{
    QVariant value;
    int effect = 0;
    int unit = 0;
    int max = 0;
    int min = 0;
    int steps = 0;

    value = conn_settings->getSetting("enable_bitmapscan");
    if (value.isValid())
        ui->ck_bitmap_scan->setChecked(value == "on");
    else
        ui->ck_tdi_scan->setEnabled(false);

    value = conn_settings->getSetting("enable_tidscan");
    if (value.isValid())
        ui->ck_tdi_scan->setChecked(value == "on");
    else
        ui->ck_tdi_scan->setEnabled(false);

    value = conn_settings->getSetting("enable_hashjoin");
    if (value.isValid())
        ui->ck_hash_joins->setChecked(value == "on");
    else
        ui->ck_hash_joins->setEnabled(false);

    value = conn_settings->getSetting("enable_indexscan");
    if (value.isValid())
        ui->ck_index_scan->setChecked(value == "on");
    else
        ui->ck_index_scan->setEnabled(false);

    value = conn_settings->getSetting("enable_sort");
    if (value.isValid())
        ui->ck_sort_steps->setChecked(value == "on");
    else
        ui->ck_sort_steps->setEnabled(false);

    value = conn_settings->getSetting("enable_nestloop");
    if (value.isValid())
        ui->ck_nested_loops->setChecked(value == "on");
    else
        ui->ck_nested_loops->setEnabled(false);

    value = conn_settings->getSetting("enable_indexonlyscan");
    if (value.isValid())
        ui->ck_index_only_scan->setChecked(value == "on");
    else
        ui->ck_index_only_scan->setEnabled(false);

    value = conn_settings->getSetting("enable_material");
    if (value.isValid())
        ui->ck_materialization->setChecked(value == "on");
    else
        ui->ck_materialization->setEnabled(false);

    value = conn_settings->getSetting("enable_seqscan");
    if (value.isValid())
        ui->ck_sequential_scan->setChecked(value == "on");
    else
        ui->ck_sequential_scan->setEnabled(false);

    value = conn_settings->getSetting("enable_hashagg");
    if (value.isValid())
        ui->ck_hash_aggregations->setChecked(value == "on");
    else
        ui->ck_hash_aggregations->setEnabled(false);

    value = conn_settings->getSetting("enable_gathermerge");
    if (value.isValid())
        ui->ck_gather_merge->setChecked(value == "on");
    else
        ui->ck_gather_merge->setEnabled(false);

    value = conn_settings->getSetting("enable_mergejoin");
    if (value.isValid())
        ui->ck_merge_join->setChecked(value.toString() == "on");
    else
        ui->ck_merge_join->setEnabled(false);

    value = conn_settings->getSetting("cpu_tuple_cost");
    ui->sp_cpu_tuple_cost->setValue(value.toDouble());

    value = conn_settings->getSetting("seq_page_cost");
    ui->sp_seq_page_cost->setValue(value.toDouble());

    value = conn_settings->getSetting("random_page_cost");
    ui->sp_random_page_cost->setValue(value.toDouble());

    value = conn_settings->getSetting("cpu_operator_cost");
    ui->sp_operator_cost->setValue(value.toDouble());

    value = conn_settings->getSetting("cpu_index_tuple_cost");
    ui->sp_cpu_index_tuple_cost->setValue(value.toDouble());

    value = conn_settings->getSetting("effective_cache_size");

    if (value.toInt() < 128) {
        // kB
        effect = value.toInt() * 8;
        unit = 0;
        min = 8;
        max = 1000;
        steps = 8;
    } else if (value.toInt() < 131072) {
        // MB
        effect = value.toInt() * 8 / 1024;
        unit = 1;
        min = 0;
        max = 1000;
        steps = 1;
    } else if (value.toInt() < 134217728) {
        // GB
        effect = value.toInt() * 8 / 1024 / 1024;
        unit = 2;
        min = 0;
        max = 1000;
        steps = 1;
    } else {
        // TB
        effect = value.toInt() * 8 / 1024 / 1024 / 1024;
        unit = 3;
        min = 0;
        max = 16;
        steps = 1;
    }

    ui->cb_unit->setCurrentIndex(unit);
    ui->sp_efective_cache_size->setMaximum(max);
    ui->sp_efective_cache_size->setMinimum(min);
    ui->sp_efective_cache_size->setSingleStep(steps);
    ui->sp_efective_cache_size->setValue(effect);

}

void DlgPlanMethods::on_cb_unit_currentIndexChanged(int index)
{
    switch(index) {
    case 0:
        ui->sp_efective_cache_size->setMaximum(1000);
        ui->sp_efective_cache_size->setMinimum(8);
        ui->sp_efective_cache_size->setSingleStep(8);
        break;
    case 1:
        ui->sp_efective_cache_size->setMaximum(1000);
        ui->sp_efective_cache_size->setMinimum(0);
        ui->sp_efective_cache_size->setSingleStep(1);
        break;
    case 2:
        ui->sp_efective_cache_size->setMaximum(1000);
        ui->sp_efective_cache_size->setMinimum(0);
        ui->sp_efective_cache_size->setSingleStep(1);
        break;
    default:
        ui->sp_efective_cache_size->setMinimum(0);
        ui->sp_efective_cache_size->setMaximum(16);
        ui->sp_efective_cache_size->setSingleStep(1);
    }
}

void DlgPlanMethods::on_buttonBox_accepted()
{
    QMessageBox msg;

    try {
        conn_settings->alterSetting("enable_bitmapscan",
                                    ui->ck_bitmap_scan->isChecked() ? "on" : "off");
        conn_settings->alterSetting("enable_tidscan",
                                    ui->ck_tdi_scan->isChecked() ? "on" : "off");
        conn_settings->alterSetting("enable_hashjoin",
                                    ui->ck_hash_joins->isChecked() ? "on" : "off");
        conn_settings->alterSetting("enable_indexscan",
                                    ui->ck_index_scan->isChecked() ? "on" : "off");
        conn_settings->alterSetting("enable_sort",
                                    ui->ck_sort_steps->isChecked() ? "on" : "off");
        conn_settings->alterSetting("enable_nestloop",
                                    ui->ck_nested_loops->isChecked() ? "on" : "off");
        conn_settings->alterSetting("enable_indexonlyscan",
                                    ui->ck_index_only_scan->isChecked() ? "on" : "off");
        conn_settings->alterSetting("enable_material",
                                    ui->ck_materialization->isChecked() ? "on" : "off");
        conn_settings->alterSetting("enable_seqscan",
                                    ui->ck_sequential_scan->isChecked() ? "on" : "off");
        conn_settings->alterSetting("enable_hashagg",
                                    ui->ck_hash_aggregations->isChecked() ? "on" : "off");
        conn_settings->alterSetting("enable_gathermerge",
                                    ui->ck_gather_merge->isChecked() ? "on" : "off");
        conn_settings->alterSetting("enable_mergejoin",
                                    ui->ck_merge_join->isChecked() ? "on" : "off");

        conn_settings->alterSetting("cpu_tuple_cost", ui->sp_cpu_tuple_cost->value());
        conn_settings->alterSetting("seq_page_cost", ui->sp_seq_page_cost->value());
        conn_settings->alterSetting("random_page_cost", ui->sp_random_page_cost->value());
        conn_settings->alterSetting("cpu_operator_cost", ui->sp_operator_cost->value());
        conn_settings->alterSetting("cpu_index_tuple_cost", ui->sp_cpu_index_tuple_cost->value());
        switch (ui->cb_unit->currentIndex()) {
        case 0:
            conn_settings->alterSetting("effective_cache_size", QString("%1").arg(ui->sp_efective_cache_size->value() / 8));
            break;
        case 1:
            conn_settings->alterSetting("effective_cache_size", QString("'%1%2'").arg(ui->sp_efective_cache_size->value()).arg("MB"));
            break;
        case 2:
            conn_settings->alterSetting("effective_cache_size", QString("'%1%2'").arg(ui->sp_efective_cache_size->value()).arg("GB"));
            break;
        case 3:
            conn_settings->alterSetting("effective_cache_size", QString("'%1%2'").arg(ui->sp_efective_cache_size->value()).arg("TB"));
            break;
        }


    } catch(SettingsException &e) {
        msg.setText(e.getMessage());
        msg.exec();
    }
}

void DlgPlanMethods::on_sp_efective_cache_size_editingFinished()
{
    int val;

    if (ui->cb_unit->currentIndex() == 0) {

        val = ui->sp_efective_cache_size->value();

        if (val % 8 != 0) {

            ui->sp_efective_cache_size->setValue(val - (val % 8) + 8);

        }
    }
}
