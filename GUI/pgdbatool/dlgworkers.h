#ifndef DLGWORKERS_H
#define DLGWORKERS_H

#include <QDialog>

namespace Ui {
class DlgWorkers;
}

class DlgWorkers : public QDialog
{
    Q_OBJECT

public:
    explicit DlgWorkers(QWidget *parent = nullptr);
    ~DlgWorkers();

private:
    Ui::DlgWorkers *ui;
};

#endif // DLGWORKERS_H
