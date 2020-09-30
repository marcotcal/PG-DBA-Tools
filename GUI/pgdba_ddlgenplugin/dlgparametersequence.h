#ifndef DLGPARAMETERSEQUENCE_H
#define DLGPARAMETERSEQUENCE_H

#include <QDialog>
#include <libpq-fe.h>
#include <parameterbase.h>

namespace Ui {
class DlgParameterSequence;
}

class DlgParameterSequence : public QDialog, ParameterBase
{
    Q_OBJECT

public:
    explicit DlgParameterSequence(PGconn *connection, EditorItem *editor, QWidget *parent = 0);
    ~DlgParameterSequence();

    QString gen_reset_sequece();
    QString gen_update_sequece();
private slots:

    void on_schema_name_currentIndexChanged(const QString &arg1);
    void on_sequence_owner_currentIndexChanged(const QString &arg1);

private:
    Ui::DlgParameterSequence *ui;

    QStringList sequences(QString owner, QString schema_name);
    QStringList tables(QString owner, QString schema_name);

};

#endif // DLGPARAMETERSEQUENCE_H
