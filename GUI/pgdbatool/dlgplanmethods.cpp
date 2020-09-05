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

    ui->me_effective_cache_size->setValue(value.toLongLong());

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

        conn_settings->alterSetting("effective_cache_size", ui->me_effective_cache_size->getValue());


    } catch(SettingsException &e) {
        msg.setText(e.getMessage());
        msg.exec();
    }
}

