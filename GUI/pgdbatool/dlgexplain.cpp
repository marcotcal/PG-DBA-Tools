#include "dlgexplain.h"
#include "ui_dlgexplain.h"

DlgExplain::DlgExplain(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgExplain)
{
    ui->setupUi(this);
}

DlgExplain::~DlgExplain()
{
    delete ui;
}

QString DlgExplain::explain()
{
    QString resp = "EXPLAIN ";
    QStringList opt;

    if (ui->ck_analyse->isChecked())
        opt << "ANALYSE";
    if (ui->ck_buffers->isChecked())
        opt << "BUFFERS";
    if (ui->ck_costs->isChecked())
        opt << "COSTS";
    if (ui->ck_timing->isChecked())
        opt << "TIMING";
    if (ui->ck_verbose->isChecked())
        opt << "VERBOSE";

    if (ui->rb_text->isChecked())
        opt << "FORMAT TEXT";
    else if (ui->rb_json->isChecked())
        opt << "FORMAT JSON";
    else if (ui->rb_xml->isChecked())
        opt << "FORMAT XML";
    else if (ui->rb_yaml->isChecked())
        opt << "FORMAT YAML";

    resp += QString("(%1)").arg(opt.join(","));

    return resp;
}

void DlgExplain::on_ck_analyse_clicked(bool checked)
{
    ui->ck_timing->setEnabled(checked);
    ui->ck_buffers->setEnabled(checked);
    if(!checked) {
        ui->ck_timing->setChecked(false);
        ui->ck_buffers->setChecked(false);
    }
}
