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

    file_name += "_" + user_name;

    if (!ui->sprint->text().trimmed().isEmpty())
        file_name += "_" + ui->sprint->text().trimmed();

    file_name += "_" + ui->task->text().trimmed();
    file_name += "_" + ui->description->text().trimmed().toLower().replace(" ","_");
    file_name += ".sql";
    createFile();
}

bool DlgCreateScript::createFile()
{
    QFile file( file_name );
    if ( file.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &file );
        stream << "/*****************************************************************" << endl;
        stream << " * Description : " << ui->description->text() << endl;
        stream << " * Author      : " << ui->author->text() << endl;
        stream << " * Sprint      : " << ui->sprint->text() << endl;
        stream << " * Task        : " << ui->sprint->text() << endl;
        stream << " *****************************************************************/" << endl;

        stream << endl << endl;

        stream << "/***" << endl;
        stream << "   NOTES:" << endl;
        stream << endl;
        stream << endl;
        stream << " ***/" << endl;
        stream << endl;
        stream << endl;
        stream << "START TRANSACTION;" << endl;
        stream << endl;        
        stream << "DO" << endl;
        stream << "$BODY$" << endl;
        stream << "BEGIN " << endl;
        stream << endl;
        stream << endl;
        stream << endl;
        stream << "END;" << endl;
        stream << "$BODY$;" << endl;
        stream << endl;
        stream << "COMMIT TRANSACTION;" << endl;
        stream << endl;
    }
}
