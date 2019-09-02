#include "querymodel.h"
#include "ui_querymodel.h"

QueryModel::QueryModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryModel)
{
    ui->setupUi(this);
}

QueryModel::~QueryModel()
{
    delete ui;
}
