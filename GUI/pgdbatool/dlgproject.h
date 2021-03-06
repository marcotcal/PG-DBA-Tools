#ifndef DLGPROJECT_H
#define DLGPROJECT_H

#include <QDialog>
#include <QComboBox>
#include "connectionsdata.h"
#include "projectdata.h"

namespace Ui {
class DlgProject;
}

class DlgProject : public QDialog
{
    Q_OBJECT

public:

    explicit DlgProject(ProjectData &project, ConnectionsData &conn, QWidget *parent = nullptr);
    ~DlgProject();

    int projectAdd();
    int projectEdit();

private slots:
    void on_bt_project_path_clicked();
    virtual void done(int res);

private slots:

    // hide exec slot
    virtual int exec() {return QDialog::exec(); }
    void on_bt_connections_clicked();

    void on_cb_development_connection_activated(int index);
    void on_cb_staging_connection_activated(int index);
    void on_cb_production_connection_activated(int index);

private:
    Ui::DlgProject *ui;
    ConnectionsData &connections;
    ProjectData &project;

    void createProject();
    void createSkeleton();
    void loadCombo(QComboBox *combo);
    void loadDBCombo(QComboBox *combo, int connection);
};

#endif // DLGPROJECT_H
