#ifndef DLGPASSWORD_H
#define DLGPASSWORD_H

#include <QDialog>

namespace Ui {
class DlgPassword;
}

class DlgPassword : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPassword(QWidget *parent = nullptr);
    ~DlgPassword();
    QString getPassword();
private slots:
    void on_bt_azure_clicked();

private:
    Ui::DlgPassword *ui;
};

#endif // DLGPASSWORD_H
