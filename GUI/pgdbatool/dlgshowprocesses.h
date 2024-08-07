#ifndef DLGSHOWPROCESSES_H
#define DLGSHOWPROCESSES_H

#include <QDialog>
#include "connectionsdata.h"

namespace Ui {
class DlgShowProcesses;
}

class DlgShowProcesses : public QDialog
{
    Q_OBJECT

public:
    explicit DlgShowProcesses( ConnectionsData &connections, int sel_connection, QWidget *parent = nullptr);
    ~DlgShowProcesses();

private slots:


    void on_btClose_clicked();

private:
    Ui::DlgShowProcesses *ui;

    ConnectionsData &connections;
    int sel_connection;
};

#endif // DLGSHOWPROCESSES_H
