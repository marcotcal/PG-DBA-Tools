#ifndef FRMFINDTEXT_H
#define FRMFINDTEXT_H

#include <QWidget>

namespace Ui {
class FrmFindText;
}

class EditorItem;

class FrmFindText : public QWidget
{
    Q_OBJECT

public:
    explicit FrmFindText(EditorItem *editor);
    ~FrmFindText();

private slots:
    void on_bt_find_clicked();

    void on_bt_close_clicked();

private:
    Ui::FrmFindText *ui;
    EditorItem *editor;
};

#endif // FRMFINDTEXT_H
