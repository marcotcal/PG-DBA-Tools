#include "dlgplanmethods.h"
#include "ui_dlgplanmethods.h"

DlgPlanMethods::DlgPlanMethods(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPlanMethods)
{
    ui->setupUi(this);
}

DlgPlanMethods::~DlgPlanMethods()
{
    delete ui;
}

void DlgPlanMethods::setPlanFlags(int flags)
{
    ui->ck_bitmap_scan->setChecked((flags & PL_BITMAP_SCAN) == PL_BITMAP_SCAN);
    ui->ck_hash_aggregations->setChecked((flags & PL_HASH_AGGREGATIONS) == PL_HASH_AGGREGATIONS);
    ui->ck_hash_joins->setChecked((flags & PL_HASH_JOINS) == PL_HASH_JOINS);
    ui->ck_index_scan->setChecked((flags & PL_INDEX_SCAN) == PL_INDEX_SCAN);
    ui->ck_index_only_scan->setChecked((flags & PL_INDEX_ONLY_SCAN) == PL_INDEX_ONLY_SCAN);
    ui->ck_materialization->setChecked((flags & PL_MATERIALIZATION) == PL_MATERIALIZATION);
    ui->ck_nested_loops->setChecked((flags & PL_NESTED_LOOPS) == PL_NESTED_LOOPS);
    ui->ck_sequential_scan->setChecked((flags & PL_SEQUENTIAL_SCAN) == PL_SEQUENTIAL_SCAN);
    ui->ck_sort_steps->setChecked((flags & PL_SORT_STEPS) == PL_SORT_STEPS);
    ui->ck_tdi_scan->setChecked((flags & PL_TDI_SCAN) == PL_TDI_SCAN);
    ui->ck_gather_merge->setChecked((flags & PL_GATHER_MERGE) == PL_GATHER_MERGE);
    ui->ck_merge_join->setChecked((flags & PL_MERGE_JOIN) == PL_MERGE_JOIN);

}

void DlgPlanMethods::setDisableFlags(int flags)
{
    ui->ck_bitmap_scan->setEnabled(!((flags & PL_BITMAP_SCAN) == PL_BITMAP_SCAN));
    ui->ck_hash_aggregations->setEnabled(!((flags & PL_HASH_AGGREGATIONS) == PL_HASH_AGGREGATIONS));
    ui->ck_hash_joins->setEnabled(!((flags & PL_HASH_JOINS) == PL_HASH_JOINS));
    ui->ck_index_scan->setEnabled(!((flags & PL_INDEX_SCAN) == PL_INDEX_SCAN));
    ui->ck_index_only_scan->setEnabled(!((flags & PL_INDEX_ONLY_SCAN) == PL_INDEX_ONLY_SCAN));
    ui->ck_materialization->setEnabled(!((flags & PL_MATERIALIZATION) == PL_MATERIALIZATION));
    ui->ck_nested_loops->setEnabled(!((flags & PL_NESTED_LOOPS) == PL_NESTED_LOOPS));
    ui->ck_sequential_scan->setEnabled(!((flags & PL_SEQUENTIAL_SCAN) == PL_SEQUENTIAL_SCAN));
    ui->ck_sort_steps->setEnabled(!((flags & PL_SORT_STEPS) == PL_SORT_STEPS));
    ui->ck_tdi_scan->setEnabled(!((flags & PL_TDI_SCAN) == PL_TDI_SCAN));
    ui->ck_gather_merge->setEnabled(!((flags & PL_GATHER_MERGE) == PL_GATHER_MERGE));
    ui->ck_merge_join->setEnabled(!((flags & PL_MERGE_JOIN) == PL_MERGE_JOIN));
}

void DlgPlanMethods::setSequentialPageCost(double cost)
{
    ui->sp_seq_page_cost->setValue(cost);
}

void DlgPlanMethods::setRandomPageCost(double cost)
{
    ui->sp_random_page_cost->setValue(cost);
}

void DlgPlanMethods::setTupleCost(double cost)
{
    ui->sp_cpu_tuple_cost->setValue(cost);
}

void DlgPlanMethods::setIndexTupleCost(double cost)
{
    ui->sp_cpu_index_tuple_cost->setValue(cost);
}

void DlgPlanMethods::setOperatorCost(double cost)
{
    ui->sp_operator_cost->setValue(cost);
}

void DlgPlanMethods::setEfectiveCashSize(int size)
{
    int value = 0;
    int unit = 0;
    int max = 0;

    if (size <= 128000) {
        value = size * 8 / 1024;
        unit = 0;
        max = 1000;
    } else if (size <= 131072000) {
        value = size * 8 / 1024 / 1024;
        unit = 1;
        max = 1000;
    } else {
        value = size * 8 / 1024 / 1024 / 1024;
        unit = 2;
        max = 16;
    }

    ui->cb_unit->setCurrentIndex(unit);
    ui->sp_efective_cache_size->setMaximum(max);
    ui->sp_efective_cache_size->setValue(value);
}

int DlgPlanMethods::getPlanFlags()
{
    int flags = 0;

    flags += ui->ck_bitmap_scan->isChecked() ? PL_BITMAP_SCAN : 0;
    flags += ui->ck_hash_aggregations->isChecked() ? PL_HASH_AGGREGATIONS : 0;
    flags += ui->ck_hash_joins->isChecked() ? PL_HASH_JOINS : 0;
    flags += ui->ck_index_scan->isChecked() ? PL_INDEX_SCAN : 0;
    flags += ui->ck_index_only_scan->isChecked() ? PL_INDEX_ONLY_SCAN : 0;
    flags += ui->ck_materialization->isChecked() ? PL_MATERIALIZATION : 0;
    flags += ui->ck_nested_loops->isChecked() ? PL_NESTED_LOOPS : 0;
    flags += ui->ck_sequential_scan->isChecked() ? PL_SEQUENTIAL_SCAN : 0;
    flags += ui->ck_sort_steps->isChecked() ? PL_SORT_STEPS : 0;
    flags += ui->ck_tdi_scan->isChecked() ? PL_TDI_SCAN : 0;
    flags += ui->ck_gather_merge->isChecked() ? PL_GATHER_MERGE : 0;
    flags += ui->ck_merge_join->isChecked() ? PL_MERGE_JOIN : 0;

    return flags;
}

double DlgPlanMethods::getSequetialPageCost()
{
    return ui->sp_seq_page_cost->value();
}

double DlgPlanMethods::getRandomPageCost()
{
    return ui->sp_random_page_cost->value();
}

double DlgPlanMethods::getTupleCost()
{
    return ui->sp_cpu_tuple_cost->value();
}

double DlgPlanMethods::getIndexTupleCost()
{
    return ui->sp_cpu_index_tuple_cost->value();
}

double DlgPlanMethods::getOperatorCost()
{
    return ui->sp_operator_cost->value();
}

int DlgPlanMethods::getEfectivenessCashSize()
{
    switch(ui->cb_unit->currentIndex()) {
    case 0:
        return ui->sp_efective_cache_size->value() / 8 * 1024;
    case 1:
        return ui->sp_efective_cache_size->value() / 8 * 1024 * 1024;
    default:
        return ui->sp_efective_cache_size->value() / 8 * 1024 * 1024 * 1024;
    }
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
