#ifndef DLGINSERT_H
#define DLGINSERT_H

#include <QDialog>
#include <libpq-fe.h>

namespace Ui {
class DlgInsert;
}

class DlgInsert : public QDialog
{
    Q_OBJECT

public:
    enum {
        NO_CONFLICT_CLAUSE,
        ON_CONFLICT_DO_NOTHING,
        ON_ONFLICT_UPDATE
    };
    explicit DlgInsert(PGconn *connection, QWidget *parent = 0);
    ~DlgInsert();

    QString schema();
    QString table();
    bool addComments();
    bool removeMandatoryWithDefaults();


private slots:

    void on_schemas_currentTextChanged(const QString &arg1);
    void on_show_views_clicked();

    void on_show_tables_clicked();

private:
    Ui::DlgInsert *ui;
    PGconn *connection;

    void getSchemas();
    void getViews(const char *schema);
    void getTables(const char *schema);
};

#endif // DLGINSERT_H
