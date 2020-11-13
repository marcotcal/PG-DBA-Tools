#ifndef DLGPARAMETERSSCHEMA_H
#define DLGPARAMETERSSCHEMA_H

#include <QDialog>
#include <libpq-fe.h>
#include <parameterbase.h>

namespace Ui {
class DlgParametersSchema;
}

class DlgParametersSchema : public QDialog, ParameterBase
{
    Q_OBJECT

public:
    explicit DlgParametersSchema(PGconn *connection, EditorItem *editor, QWidget *parent = 0);
    ~DlgParametersSchema();

    QString gen_create_schema();
    QString gen_drop_schema();
private slots:
    void on_schema_owner_currentIndexChanged(const QString &arg1);

private:
    Ui::DlgParametersSchema *ui;

};

#endif // DLGPARAMETERSSCHEMA_H
