#ifndef DLGPARAMETERSEQUENCE_H
#define DLGPARAMETERSEQUENCE_H

#include <QDialog>
#include <libpq-fe.h>

namespace Ui {
class DlgParameterSequence;
}

class DlgParameterSequence : public QDialog
{
    Q_OBJECT

public:
    explicit DlgParameterSequence(PGconn *connection, QWidget *parent = 0);
    ~DlgParameterSequence();

    QString schemaName();
    QString objectOwner();
    QString objectName();

    void setUserList(QStringList values);
    void setSchemas(QStringList values);
    void setObjecs(QStringList values);
private slots:
    void on_schema_name_currentIndexChanged(int index);

    void on_object_owner_currentIndexChanged(int index);

private:
    Ui::DlgParameterSequence *ui;
    PGconn *connection;

    void objectsList();
};

#endif // DLGPARAMETERSEQUENCE_H
