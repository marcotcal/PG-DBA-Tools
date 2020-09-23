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

    void setOffset(int value);
    void setUserList(QStringList values);
    void setSchemas(QStringList values);

    QString gen_create_schema();
    QString gen_drop_schema();
private slots:
    void on_schema_owner_currentIndexChanged(const QString &arg1);

private:
    Ui::DlgParametersSchema *ui;
    PGconn *connection;
    QString error;
    QStringList names;
    QStringList create_schemas;
    QStringList drop_schemas;
    int offset;
    void readSchemas();

};

#endif // DLGPARAMETERSSCHEMA_H
