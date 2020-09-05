#ifndef DLGINSERT_H
#define DLGINSERT_H

#include <QDialog>

namespace Ui {
class DlgInsert;
}

class DlgInsert : public QDialog
{
    Q_OBJECT

public:
    explicit DlgInsert(QWidget *parent = 0);
    ~DlgInsert();

private:
    Ui::DlgInsert *ui;
};

#endif // DLGINSERT_H
