#ifndef FRMPROJECTSCRIPT_H
#define FRMPROJECTSCRIPT_H

#include <QWidget>
#include <libpq-fe.h>
#include "projectdata.h"

namespace Ui {
class FrmProjectScript;
}

class FrmProjectScript : public QWidget
{
    Q_OBJECT

public:
    explicit FrmProjectScript(ProjectData &project, QWidget *parent = 0);
    ~FrmProjectScript();

    void loadRunFiles();
    void loadReviewFiles();
    void loadErrorFiles();
    void loadDoneFiles();

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_bt_run_run_clicked();
    void on_bt_run_open_clicked();
    void on_bt_review_run_clicked();
    void on_bt_review_open_clicked();
    void on_bt_show_error_clicked();

signals:

    void openFileOnSQLTool(QString file_name);

private:
    Ui::FrmProjectScript *ui;

    ProjectData &project;
    QString run_dir;
    QString review_dir;
    QString done_dir;
    QString error_dir;

    bool executeScript();
};

#endif // FRMPROJECTSCRIPT_H
