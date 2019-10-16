#include "outputset.h"
#include "ui_outputset.h"

OutputSet::OutputSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputSet)
{
    ui->setupUi(this);
}

OutputSet::~OutputSet()
{
    delete ui;
}

void OutputSet::on_bt_txt_clicked()
{
    ui->output_stack->setCurrentIndex(0);
}

void OutputSet::on_bt_json_clicked()
{
    ui->output_stack->setCurrentIndex(0);
}

void OutputSet::on_bt_xml_clicked()
{
    ui->output_stack->setCurrentIndex(0);
}

void OutputSet::on_bt_grid_clicked()
{
    ui->output_stack->setCurrentIndex(1);
}

void OutputSet::on_bt_html_clicked()
{
    ui->output_stack->setCurrentIndex(2);
}
