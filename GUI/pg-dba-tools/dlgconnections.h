#ifndef DLGCONNECTIONS_H
#define DLGCONNECTIONS_H

#include <QDialog>

namespace Ui {
class DlgConnections;
}

class DlgConnections : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnections(QWidget *parent = 0);
    ~DlgConnections();

private:
    Ui::DlgConnections *ui;
};

#endif // DLGCONNECTIONS_H
