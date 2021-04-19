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
        ON_CONFLICT_UPDATE
    };
    explicit DlgInsert(QWidget *parent = 0);
    ~DlgInsert();

   bool getAddComments();
   bool getFieldTypes();
   bool getOnlyMandatory();
   bool getRemoveMandatoryWithDefaults();
   int getOnConflict();

private slots:


   void on_add_comments_toggled(bool checked);

private:
    Ui::DlgInsert *ui;

};

#endif // DLGINSERT_H
