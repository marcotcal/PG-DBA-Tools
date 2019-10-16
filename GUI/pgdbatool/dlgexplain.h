#ifndef DLGEXPLAIN_H
#define DLGEXPLAIN_H

#include <QDialog>

namespace Ui {
class DlgExplain;
}

class DlgExplain : public QDialog
{
    Q_OBJECT

public:
    explicit DlgExplain(QWidget *parent = 0);
    ~DlgExplain();
    QString explain();
private slots:
    void on_ck_analyse_clicked(bool checked);

private:
    Ui::DlgExplain *ui;
};

#endif // DLGEXPLAIN_H
