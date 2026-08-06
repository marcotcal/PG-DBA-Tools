#include "dlgpassword.h"
#include "ui_dlgpassword.h"
#include <QProcess>
#include <QStringList>
#include <QDebug>

DlgPassword::DlgPassword(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgPassword)
{
    ui->setupUi(this);
}

DlgPassword::~DlgPassword()
{
    delete ui;
}

QString DlgPassword::getPassword()
{
    return ui->textEdit->toPlainText();
}

void DlgPassword::on_bt_azure_clicked()
{
    QProcess process;

    // Define o comando e os argumentos separadamente
    QString program = "az";
    QStringList arguments;
    arguments << "account" << "get-access-token"
              << "--resource-type" << "oss-rdbms"
              << "-o" << "tsv"
              << "--query" << "accessToken";

    // Inicia o processo
    process.start(program, arguments);

    // Aguarda o término da execução (bloqueante)
    if (process.waitForFinished()) {
        // Verifica se o comando retornou erro (exit code != 0)
        if (process.exitCode() == 0) {
            // Captura a saída padrão e remove espaços/quebras de linha
            QString accessToken = QString::fromUtf8(process.readAllStandardOutput()).trimmed();
            ui->textEdit->setPlainText(accessToken);
        } else {
            // Captura a mensagem de erro da CLI
            QString errorMsg = QString::fromUtf8(process.readAllStandardError());
            ui->textEdit->setPlainText(errorMsg);
        }
    } else {
        ui->textEdit->setPlainText(process.errorString());
    }
}

