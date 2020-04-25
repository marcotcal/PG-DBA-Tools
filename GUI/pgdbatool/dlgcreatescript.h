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
    explicit DlgCreateScript(QWidget *parent = nullptr);
    ~DlgCreateScript();

    QString getFileName() { return file_name; }

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DlgCreateScript *ui;

    QString file_name;
};

#endif // DLGCREATESCRIPT_H
