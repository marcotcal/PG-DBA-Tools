#ifndef DLGPARAMETERS_H
#define DLGPARAMETERS_H

#include <QDialog>
#include "querymodeldata.h"
#include "connectionsdata.h"

namespace Ui {
class DlgParameters;
}

class DlgParameters : public QDialog
{
    Q_OBJECT

public:
    explicit DlgParameters(QueryModelData *model_data, ConnectionsData &connections,
                           int sel_connection, QWidget *parent = nullptr);
    ~DlgParameters();
    QString getSelectedDatabaseName();

private slots:
   void on_buttonBox_accepted();

private:
    Ui::DlgParameters *ui;
    QueryModelData *model_data;
    void loadParameters();
    void assignValues();
    ConnectionsData &connections;
    int sel_connection;

    void loadDatabases();
};

#endif // DLGPARAMETERS_H
