#ifndef DLGTRANSACTION_H
#define DLGTRANSACTION_H

#include <QDialog>

namespace Ui {
class DlgTransaction;
}

class DlgTransaction : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTransaction(QWidget *parent = nullptr);
    ~DlgTransaction();
    QString &getCommand() { return command; }
private slots:
    void on_buttonBox_accepted();

private:
    Ui::DlgTransaction *ui;
    QString command;
};

#endif // DLGTRANSACTION_H
