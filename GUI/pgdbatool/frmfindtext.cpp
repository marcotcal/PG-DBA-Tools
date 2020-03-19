#include "frmfindtext.h"
#include "ui_frmfindtext.h"
#include "sqltool.h"

FrmFindText::FrmFindText(EditorItem *editor) :
    QWidget(nullptr),
    editor(editor),
    ui(new Ui::FrmFindText)
{
    ui->setupUi(this);
}

FrmFindText::~FrmFindText()
{
    delete ui;
}

void FrmFindText::on_bt_find_clicked()
{
    editor->findFirst(ui->text_to_find->text(), false, true, false, false);
}

void FrmFindText::on_bt_close_clicked()
{
    close();
}
