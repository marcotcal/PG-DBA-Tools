#include "dlgplanmethods.h"
#include "ui_dlgplanmethods.h"

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

    value = conn_settings->getSetting("enable_bitmapscan", ConnectionSettings::st_setting);
    if (value.isValid())
        ui->ck_bitmap_scan->setChecked(value == "on");
    else
        ui->ck_tdi_scan->setEnabled(false);

    value = conn_settings->getSetting("enable_tidscan", ConnectionSettings::st_setting);
    if (value.isValid())
        ui->ck_tdi_scan->setChecked(value == "on");
    else
        ui->ck_tdi_scan->setEnabled(false);

    value = conn_settings->getSetting("enable_hashjoin", ConnectionSettings::st_setting);
    if (value.isValid())
        ui->ck_hash_joins->setChecked(value == "on");
    else
        ui->ck_hash_joins->setEnabled(false);

    value = conn_settings->getSetting("enable_indexscan", ConnectionSettings::st_setting);
    if (value.isValid())
        ui->ck_index_scan->setChecked(value == "on");
    else
        ui->ck_index_scan->setEnabled(false);

    value = conn_settings->getSetting("enable_sort", ConnectionSettings::st_setting);
    if (value.isValid())
        ui->ck_sort_steps->setChecked(value == "on");
    else
        ui->ck_sort_steps->setEnabled(false);

    value = conn_settings->getSetting("enable_nestloop", ConnectionSettings::st_setting);
    if (value.isValid())
        ui->ck_nested_loops->setChecked(value == "on");
    else
        ui->ck_nested_loops->setEnabled(false);

    value = conn_settings->getSetting("enable_indexonlyscan", ConnectionSettings::st_setting);
    if (value.isValid())
        ui->ck_index_only_scan->setChecked(value == "on");
    else
        ui->ck_index_only_scan->setEnabled(false);

    value = conn_settings->getSetting("enable_material", ConnectionSettings::st_setting);
    if (value.isValid())
        ui->ck_materialization->setChecked(value == "on");
    else
        ui->ck_materialization->setEnabled(false);

    value = conn_settings->getSetting("enable_seqscan", ConnectionSettings::st_setting);
    if (value.isValid())
        ui->ck_sequential_scan->setChecked(value == "on");
    else
        ui->ck_sequential_scan->setEnabled(false);

    value = conn_settings->getSetting("enable_hashagg", ConnectionSettings::st_setting);
    if (value.isValid())
        ui->ck_hash_aggregations->setChecked(value == "on");
    else
        ui->ck_hash_aggregations->setEnabled(false);

    value = conn_settings->getSetting("enable_gathermerge", ConnectionSettings::st_setting);
    if (value.isValid())
        ui->ck_gather_merge->setChecked(value == "on");
    else
        ui->ck_gather_merge->setEnabled(false);

    value = conn_settings->getSetting("enable_mergejoin", ConnectionSettings::st_setting);
    if (value.isValid())
        ui->ck_merge_join->setChecked(value.toString() == "on");
    else
        ui->ck_merge_join->setEnabled(false);

    value = conn_settings->getSetting("cpu_tuple_cost", ConnectionSettings::st_setting);
    ui->sp_cpu_tuple_cost->setValue(value.toDouble());

    value = conn_settings->getSetting("seq_page_cost", ConnectionSettings::st_setting);
    ui->sp_seq_page_cost->setValue(value.toDouble());

    value = conn_settings->getSetting("random_page_cost", ConnectionSettings::st_setting);
    ui->sp_random_page_cost->setValue(value.toDouble());

    value = conn_settings->getSetting("cpu_operator_cost", ConnectionSettings::st_setting);
    ui->sp_operator_cost->setValue(value.toDouble());

    value = conn_settings->getSetting("random_page_cost", ConnectionSettings::st_setting);
    ui->sp_random_page_cost->setValue(value.toDouble());

    value = conn_settings->getSetting("cpu_index_tuple_cost", ConnectionSettings::st_setting);
    ui->sp_cpu_index_tuple_cost->setValue(value.toDouble());

    value = conn_settings->getSetting("effective_cache_size", ConnectionSettings::st_setting);

    if (value.toInt() <= 128000) {
        effect = value.toInt() * 8 / 1024;
        unit = 0;
        max = 1000;
    } else if (value.toInt() <= 131072000) {
        effect = value.toInt() * 8 / 1024 / 1024;
        unit = 1;
        max = 1000;
    } else {
        effect = value.toInt() * 8 / 1024 / 1024 / 1024;
        unit = 2;
        max = 16;
    }

    ui->cb_unit->setCurrentIndex(unit);
    ui->sp_efective_cache_size->setMaximum(max);
    ui->sp_efective_cache_size->setValue(effect);

}

void DlgPlanMethods::on_cb_unit_currentIndexChanged(int index)
{
    switch(index) {
    case 0:
    case 1:
        ui->sp_efective_cache_size->setMaximum(1000);
        break;
    default:
        ui->sp_efective_cache_size->setMaximum(16);
    }
}
