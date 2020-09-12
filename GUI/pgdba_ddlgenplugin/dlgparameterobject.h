#ifndef DLGPARAMETEROBJECT_H
#define DLGPARAMETEROBJECT_H

#include <QDialog>

namespace Ui {
class DlgParameterObject;
}

class DlgParameterObject : public QDialog
{
    Q_OBJECT

public:
    explicit DlgParameterObject(QWidget *parent = 0);
    ~DlgParameterObject();

private:
    Ui::DlgParameterObject *ui;
};

#endif // DLGPARAMETEROBJECT_H
