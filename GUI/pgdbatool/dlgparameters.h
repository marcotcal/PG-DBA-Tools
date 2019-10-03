#ifndef DLGPARAMETERS_H
#define DLGPARAMETERS_H

#include <QDialog>
#include "querymodeldata.h"

namespace Ui {
class DlgParameters;
}

class DlgParameters : public QDialog
{
    Q_OBJECT

public:
    explicit DlgParameters(QueryModelData *data, QWidget *parent = nullptr);
    ~DlgParameters();
private slots:
   void on_buttonBox_accepted();

private:
    Ui::DlgParameters *ui;
    QueryModelData *data;
    void loadParameters();
    void assignValues();
};

#endif // DLGPARAMETERS_H
