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
    explicit DlgInsert(PGconn *connection, QWidget *parent = 0);
    ~DlgInsert();

    void getSchemas();
    void getViews(const char *schema);
    void getTables(const char *schema);

private slots:

    void on_schemas_currentTextChanged(const QString &arg1);
    void on_show_views_clicked();

    void on_show_tables_clicked();

private:
    Ui::DlgInsert *ui;
    PGconn *connection;
};

#endif // DLGINSERT_H
