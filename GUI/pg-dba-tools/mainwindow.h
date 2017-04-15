#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dmoconnection.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loadDatabases(void);

    DMOConnection *getDatabase() const;
    void setDatabase(DMOConnection *value);

private slots:
    void on_actionExit_triggered();
    void on_actionConnections_triggered();

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private:
    Ui::MainWindow *ui;    
    DMOConnection *database;
};

#endif // MAINWINDOW_H
