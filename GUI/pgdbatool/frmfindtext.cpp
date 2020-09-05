#include "frmfindtext.h"
#include "ui_frmfindtext.h"
#include "sqltool.h"

FrmFindText::FrmFindText(EditorItem *editor) :
    QWidget(nullptr),
    ui(new Ui::FrmFindText),
    editor(editor)
{
    ui->setupUi(this);
}

FrmFindText::~FrmFindText()
{
    delete ui;
}

void FrmFindText::on_bt_find_clicked()
{
    editor->findFirst(ui->text_to_find->text(),
                      ui->regular_expressions->isChecked(),
                      ui->case_insensitive->isChecked(),
                      ui->whole_words->isChecked(),
                      ui->warp_around->isChecked(),
                      ui->forward->isChecked(),
                      ui->cursor_position ? -1 : ui->line->value(),
                      ui->cursor_position ? -1 : 0,
                      true, // text found will be always visible
                      ui->posix_regular_expressions->isChecked());
}

void FrmFindText::on_bt_close_clicked()
{
    close();
}

void FrmFindText::on_cursor_position_toggled(bool checked)
{
    ui->label_line->setEnabled(!checked);
    ui->line->setEnabled(!checked);
}
