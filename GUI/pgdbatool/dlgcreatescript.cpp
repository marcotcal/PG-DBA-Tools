#include "dlgcreatescript.h"
#include "ui_dlgcreatescript.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>

DlgCreateScript::DlgCreateScript(QString path_to_file, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgCreateScript),
    path_to_file(path_to_file)
{
    ui->setupUi(this);
}

DlgCreateScript::~DlgCreateScript()
{
    delete ui;
}

QString DlgCreateScript::getDescription()
{
    return ui->description->text();
}

QString DlgCreateScript::getAuthor()
{
    return ui->author->text();
}

QString DlgCreateScript::getSprint()
{
    return ui->sprint->text();
}

QString DlgCreateScript::getTask()
{
    return ui->task->text();
}

void DlgCreateScript::on_buttonBox_accepted()
{
    QString user_name;
    file_name =  path_to_file + QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    user_name = qgetenv("USER");
    if (user_name.isEmpty())
        user_name = qgetenv("USERNAME");

    file_name += "-U_" + user_name;

    if (!ui->sprint->text().trimmed().isEmpty())
        file_name += "-S_" + ui->sprint->text().trimmed();

    file_name += "-TSK_" + ui->task->text().trimmed();
    file_name += "-" + ui->description->text().trimmed().toLower().replace(" ","_");
    file_name += ".sql";
    createFile();
}

bool DlgCreateScript::createFile()
{
    QFile file( file_name );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << "/*****************************************************************" << Qt::endl;
        stream << " * Description : " << ui->description->text() << Qt::endl;
        stream << " * Author      : " << ui->author->text() << Qt::endl;
        stream << " * Sprint      : " << ui->sprint->text() << Qt::endl;
        stream << " * Task        : " << ui->sprint->text() << Qt::endl;
        stream << " *****************************************************************/" << Qt::endl;

        stream << Qt::endl << Qt::endl;

        stream << "/***" << Qt::endl;
        stream << "   NOTES:" << Qt::endl;
        stream << Qt::endl;
        stream << Qt::endl;
        stream << " ***/" << Qt::endl;
        stream << Qt::endl;
        stream << Qt::endl;
        stream << "START TRANSACTION;" << Qt::endl;
        stream << Qt::endl;
        stream << "DO" << Qt::endl;
        stream << "$BODY$" << Qt::endl;
        stream << "BEGIN " << Qt::endl;
        stream << Qt::endl;
        stream << Qt::endl;
        stream << Qt::endl;
        stream << "END;" << Qt::endl;
        stream << "$BODY$;" << Qt::endl;
        stream << Qt::endl;
        stream << "COMMIT TRANSACTION;" << Qt::endl;
        stream << Qt::endl;

        return true;
    }

    return false;
}
