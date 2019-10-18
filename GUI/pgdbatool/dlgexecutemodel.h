#ifndef DLGEXECUTEMODEL_H
#define DLGEXECUTEMODEL_H

#include <QDialog>

namespace Ui {
class DlgExecuteModel;
}

class ModelItem : public QObject
{

    Q_OBJECT

public:

private:


};

class DlgExecuteModel : public QDialog
{
    Q_OBJECT

public:
    explicit DlgExecuteModel(QWidget *parent = nullptr);
    ~DlgExecuteModel();

private:
    Ui::DlgExecuteModel *ui;
    void readModels();

};

#endif // DLGEXECUTEMODEL_H
