#include "dlgplugins.h"
#include "ui_dlgplugins.h"

DlgPlugins::DlgPlugins(QMap<QString, PGDBAPluginInterface *> &interface_list, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPlugins)
{
    ui->setupUi(this);
    QTableWidgetItem *plugin_name;
    QTableWidgetItem *version;
    QTableWidgetItem *file_name;

    ui->plugin_table->setColumnWidth(0,150);
    ui->plugin_table->setColumnWidth(1,600);
    ui->plugin_table->setColumnWidth(2,100);

    QMapIterator<QString, PGDBAPluginInterface*> i(interface_list);
    while (i.hasNext()) {
        i.next();
        ui->plugin_table->setRowCount(ui->plugin_table->rowCount()+1);
        plugin_name = new QTableWidgetItem(i.value()->plugin_name());
        version = new QTableWidgetItem(i.value()->version());
        file_name = new QTableWidgetItem(i.value()->fileName());

        ui->plugin_table->setItem(ui->plugin_table->rowCount()-1, 0, plugin_name);
        ui->plugin_table->setItem(ui->plugin_table->rowCount()-1, 1, file_name);
        ui->plugin_table->setItem(ui->plugin_table->rowCount()-1, 2, version);

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
