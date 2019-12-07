#ifndef DLGPROJECT_H
#define DLGPROJECT_H

#include <QDialog>

namespace Ui {
class DlgProject;
}

class DlgProject : public QDialog
{
    Q_OBJECT

public:
    explicit DlgProject(QWidget *parent = nullptr);
    ~DlgProject();

private:
    Ui::DlgProject *ui;
};

#endif // DLGPROJECT_H
