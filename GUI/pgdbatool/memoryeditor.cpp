#include "memoryeditor.h"
#include "ui_memoryeditor.h"
#include <math.h>

MemoryEditor::MemoryEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MemoryEditor)
{
    ui->setupUi(this);
}

MemoryEditor::~MemoryEditor()
{
    delete ui;
}

void MemoryEditor::setValue(qlonglong size)
{
    mem_params params;

    if (size < 128) {
        // kB
        params.size = size * 8;
        params.unit = MS_KB;
        params.min = 8;
        params.max = 1000;
        params.step = 8;
    } else if (size < 131072) {
        // MB
        params.size = size * 8 / 1024;
        params.unit = MS_MB;
        params.min = 0;
        params.max = 1000;
        params.step = 1;
    } else if (size < 134217728) {
        // GB
        params.size = static_cast<long>(size * 8 / pow(1024,2));
        params.unit = MS_GB;
        params.min = 0;
        params.max = 1000;
        params.step = 1;
    } else {
        // TB
        params.size = static_cast<long>(size * 8 / pow(1024,3));;
        params.unit = MS_TB;
        params.min = 0;
        params.max = 15;
        params.step = 1;
    }

    ui->spinner->setMaximum(params.max);
    ui->spinner->setMinimum(params.min);
    ui->spinner->setSingleStep(params.step);
    ui->spinner->setValue(static_cast<int>(params.size));
    ui->combo->setCurrentIndex(params.unit);
}

qlonglong MemoryEditor::getValue()
{
    long mem_size;

    mem_size = static_cast<int>(
                floor(static_cast<double>(ui->spinner->value()) / 8 * pow(1024, ui->combo->currentIndex())));

    return mem_size;
}

void MemoryEditor::on_combo_currentIndexChanged(int index)
{
    switch(index) {
    case 0:
        ui->spinner->setMaximum(1000);
        ui->spinner->setMinimum(8);
        ui->spinner->setSingleStep(8);
        break;
    case 1:
        ui->spinner->setMaximum(1000);
        ui->spinner->setMinimum(0);
        ui->spinner->setSingleStep(1);
        break;
    case 2:
        ui->spinner->setMaximum(1000);
        ui->spinner->setMinimum(0);
        ui->spinner->setSingleStep(1);
        break;
    default:
        ui->spinner->setMinimum(0);
        ui->spinner->setMaximum(15);
        ui->spinner->setSingleStep(1);
    }
}

void MemoryEditor::on_spinner_editingFinished()
{
    int val;

    if (ui->combo->currentIndex() == 0) {

        val = ui->spinner->value();

        if (val % 8 != 0) {

            ui->spinner->setValue(val - (val % 8) + 8);

        }
    }
}
