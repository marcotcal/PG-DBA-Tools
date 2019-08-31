#ifndef DLGMENUNEW_H
#define DLGMENUNEW_H

#include <QDialog>

namespace Ui {
class DlgMenuNew;
}

class DlgMenuNew : public QDialog
{
    Q_OBJECT

public:
    explicit DlgMenuNew(QWidget *parent = nullptr);
    ~DlgMenuNew();

private:
    Ui::DlgMenuNew *ui;
};

#endif // DLGMENUNEW_H
