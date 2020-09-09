#include "dlgplugins.h"
#include "ui_dlgplugins.h"

DlgPlugins::DlgPlugins(QMap<QString, PGDBAPluginInterface *> &interface_list, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPlugins)
{
    ui->setupUi(this);

    ui->plugin_table->setColumnWidth(0,150);
    ui->plugin_table->setColumnWidth(1,350);
    ui->plugin_table->setColumnWidth(2,100);
    ui->plugin_table->setRowCount(1);
    QMapIterator<QString, PGDBAPluginInterface*> i(interface_list);
    while (i.hasNext()) {
        i.next();



        //cout << i.key() << ": " << i.value() << Qt::endl;
    }
}

DlgPlugins::~DlgPlugins()
{
    delete ui;
}

void DlgPlugins::on_bt_close_clicked()
{
    accept();
}
