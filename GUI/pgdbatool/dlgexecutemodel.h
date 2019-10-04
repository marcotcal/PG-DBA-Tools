#ifndef DLGEXECUTEMODEL_H
#define DLGEXECUTEMODEL_H

#include <QDialog>

namespace Ui {
class DlgExecuteModel;
}

class DlgExecuteModel : public QDialog
{
    Q_OBJECT

public:
    explicit DlgExecuteModel(QWidget *parent = nullptr);
    ~DlgExecuteModel();

private:
    Ui::DlgExecuteModel *ui;
};

#endif // DLGEXECUTEMODEL_H
