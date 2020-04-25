#ifndef FRMPROJECTSCRIPT_H
#define FRMPROJECTSCRIPT_H

#include <QWidget>
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

private:
    Ui::FrmProjectScript *ui;

    ProjectData &project;
};

#endif // FRMPROJECTSCRIPT_H
