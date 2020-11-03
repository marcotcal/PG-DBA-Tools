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
    QString objectName();

    void setLabel(QString value);
    void setUserList(QStringList values);
    void setSchemas(QStringList values);
    void setObjecs(QStringList values);
private slots:
    void on_schema_name_currentIndexChanged(int index);

    void on_object_owner_currentIndexChanged(int index);

private:
    Ui::DlgParameterObject *ui;
    PGconn *connection;

    void objectsList();
};

#endif // DLGPARAMETEROBJECT_H
