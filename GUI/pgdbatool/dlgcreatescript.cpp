#include "dlgcreatescript.h"
#include "ui_dlgcreatescript.h"
#include <QDateTime>

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

void DlgCreateScript::on_buttonBox_accepted()
{
    file_name = QDateTime::currentDateTime().toString("yyyyMMddhhmmss") +
            "_" + ui->sprint->text().trimmed() +
            "_" + ui->task->text().trimmed() +
            "_" + ui->description->text().trimmed().toLower().replace(" ","_") +
            ".sql";
}
