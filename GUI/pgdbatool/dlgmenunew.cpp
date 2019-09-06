#include "dlgmenunew.h"
#include "ui_dlgmenunew.h"

DlgMenuNew::DlgMenuNew(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgMenuNew)
{
    ui->setupUi(this);
    QListWidgetItem *item;

    item = new QListWidgetItem("SQL Tool");
    item->setIcon(QIcon(":/icons/images/icons/icons8-upload-link-document-40.png"));
    item->setData(Qt::UserRole, QVariant("<html><body>"
                                         " <h2><img src=':/icons/images/icons/icons8-upload-link-document-40.png'> SQL Tool</h2>"
                                         "Open a new multi-editor query tool. Each Query Tool will have it "
                                         "own connection and transaction context.<br>"
                                         "All editors of a SQL Tool will will be at the same transaction."
                                          "</body></html>"));
    ui->editors_list->addItem(item);

    item = new QListWidgetItem("Query Models");
    item->setIcon(QIcon(":/icons/images/icons/icons8-data-quality-40.png"));
    item->setData(Qt::UserRole, QVariant("<html><body>"
                                         " <h2><img src=':/icons/images/icons/icons8-data-quality-40.png'> Query Models</h2>"
                                         "Open a new Query Model. The query models are the templates for the queries used to analize the . "
                                         "database. The query model files defines the query itself and describe the parameters the user "
                                         "must supply when the query runs."
                                         "</body></html>"));

    ui->editors_list->addItem(item);
    ui->editors_list->setCurrentRow(0);
}

DlgMenuNew::~DlgMenuNew()
{
    delete ui;
}

int DlgMenuNew::getSelection()
{
    return ui->editors_list->currentRow();
}

void DlgMenuNew::on_editors_list_currentRowChanged(int currentRow)
{
    QString about;

    about = ui->editors_list->item(currentRow)->data(Qt::UserRole).toString();
    ui->editor_about->setText(about);
}
