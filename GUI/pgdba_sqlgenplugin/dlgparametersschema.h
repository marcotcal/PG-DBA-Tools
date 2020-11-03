#ifndef DLGPARAMETERSSCHEMA_H
#define DLGPARAMETERSSCHEMA_H

#include <QDialog>
#include <libpq-fe.h>

namespace Ui {
class DlgParametersSchema;
}

class DlgParametersSchema : public QDialog
{
    Q_OBJECT

public:
    explicit DlgParametersSchema(PGconn *connection, QWidget *parent = 0);
    ~DlgParametersSchema();

    QString schemaName();
    QString schemaOwner();

    void setUserList(QStringList values);
    void setSchemas(QStringList values);
private:
    Ui::DlgParametersSchema *ui;
    PGconn *connection;

};

#endif // DLGPARAMETERSSCHEMA_H
