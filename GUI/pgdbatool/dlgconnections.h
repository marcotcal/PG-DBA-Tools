#ifndef DLGCONNECTIONS_H
#define DLGCONNECTIONS_H
#include <libpq-fe.h>
#include <QDialog>
#include <QWidget>

namespace Ui {
class DlgConnections;
}

class DlgConnections : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnections(QWidget *parent = 0);
    ~DlgConnections();

private slots:
    void on_bt_close_clicked();
    void on_bt_add_connection_clicked();
    void on_bt_delete_connection_clicked();

private:
    Ui::DlgConnections *ui;
    PGconn *connection;
};

#endif // DLGCONNECTIONS_H
