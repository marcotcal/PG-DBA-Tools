#ifndef FRMFINDTEXT_H
#define FRMFINDTEXT_H

#include <QWidget>

namespace Ui {
class FrmFindText;
}

class FrmFindText : public QWidget
{
    Q_OBJECT

public:
    explicit FrmFindText(QWidget *parent = nullptr);
    ~FrmFindText();

private slots:
    void on_bt_find_clicked();

    void on_bt_close_clicked();

private:
    Ui::FrmFindText *ui;
};

#endif // FRMFINDTEXT_H
