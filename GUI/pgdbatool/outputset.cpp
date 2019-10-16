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
