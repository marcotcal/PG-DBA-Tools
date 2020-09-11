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

private:
    Ui::DlgParametersSchema *ui;
    PGconn *connection;

    QStringList schemas();
    QStringList users();
};

#endif // DLGPARAMETERSSCHEMA_H
