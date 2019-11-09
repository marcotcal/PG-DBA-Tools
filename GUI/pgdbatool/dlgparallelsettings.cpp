#include "dlgparallelsettings.h"
#include "ui_dlgparallelsettings.h"

DlgParallelSettings::DlgParallelSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParallelSettings)
{
    ui->setupUi(this);
}

DlgParallelSettings::~DlgParallelSettings()
{
    delete ui;
}
