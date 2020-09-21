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

    void setUserList(QStringList values);
    void setSchemas(QStringList values);

    QString gen_create_schema(PGconn *connection, int offset);
    QString gen_drop_schema(PGconn *connection, int offset);
private:
    Ui::DlgParametersSchema *ui;
    PGconn *connection;

    QString error;

};

#endif // DLGPARAMETERSSCHEMA_H
