#ifndef DLGCREATESCRIPT_H
#define DLGCREATESCRIPT_H

#include <QDialog>

namespace Ui {
class DlgCreateScript;
}

class DlgCreateScript : public QDialog
{
    Q_OBJECT

public:
    explicit DlgCreateScript(QString path_to_file, QWidget *parent = nullptr);
    ~DlgCreateScript();

    QString getFileName() { return file_name; }
    QString getDescription();
    QString getAuthor();
    QString getSprint();
    QString getTask();

private slots:

    void on_buttonBox_accepted();

private:
    Ui::DlgCreateScript *ui;

    QString file_name;
    QString path_to_file;

    bool createFile();
};

#endif // DLGCREATESCRIPT_H
