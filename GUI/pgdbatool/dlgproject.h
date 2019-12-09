#ifndef DLGPROJECT_H
#define DLGPROJECT_H

#include <QDialog>
#include <QComboBox>
#include "connectionsdata.h"

namespace Ui {
class DlgProject;
}

class DlgProject : public QDialog
{
    Q_OBJECT

public:
    explicit DlgProject(ConnectionsData &conn, QWidget *parent = nullptr);
    ~DlgProject();

    void createProject();
private slots:
    void on_bt_project_path_clicked();

private:
    Ui::DlgProject *ui;
    ConnectionsData &connections;

    void createSkeleton(QString dir);
    void loadConnections(QComboBox *combo);
};

#endif // DLGPROJECT_H
