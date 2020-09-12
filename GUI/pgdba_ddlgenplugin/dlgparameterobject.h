#ifndef DLGPARAMETEROBJECT_H
#define DLGPARAMETEROBJECT_H

#include <QDialog>
#include <libpq-fe.h>

namespace Ui {
class DlgParameterObject;
}

class DlgParameterObject : public QDialog
{
    Q_OBJECT

public:
    explicit DlgParameterObject(PGconn *connection, QWidget *parent = 0);
    ~DlgParameterObject();

    QString schemaName();
    QString objectOwner();

    void setLabel(QString value);
    void setUserList(QStringList values);
    void setSchemas(QStringList values);
    void setObjecs(QStringList values);
private:
    Ui::DlgParameterObject *ui;
    PGconn *connection;
};

#endif // DLGPARAMETEROBJECT_H
