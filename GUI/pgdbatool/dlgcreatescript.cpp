#include "dlgcreatescript.h"
#include "ui_dlgcreatescript.h"

DlgCreateScript::DlgCreateScript(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCreateScript)
{
    ui->setupUi(this);
}

DlgCreateScript::~DlgCreateScript()
{
    delete ui;
}
