#ifndef DLGUPDATE_H
#define DLGUPDATE_H

#include <QDialog>

namespace Ui {
class DlgUpdate;
}

class DlgUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit DlgUpdate(QWidget *parent = 0);
    ~DlgUpdate();

    bool getAddComments();
    bool getFieldTypes();

private slots:
    void on_add_comments_toggled(bool checked);

private:
    Ui::DlgUpdate *ui;
};

#endif // DLGUPDATE_H
