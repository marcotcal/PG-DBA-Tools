#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <libpq-fe.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();
    void on_actionConnections_triggered();

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private:
    Ui::MainWindow *ui;
    PGconn *conn;
    int     status;
    void loadDatabases(void);
};

#endif // MAINWINDOW_H
