#include "dlgplugins.h"
#include "ui_dlgplugins.h"

DlgPlugins::DlgPlugins(QMap<QString, PluginElement *> &interface_list, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPlugins),
    interface_list(interface_list)
{
    ui->setupUi(this);
    loadTable();
    ui->plugin_table->selectRow(0);
}

DlgPlugins::~DlgPlugins()
{
    delete ui;
}

void DlgPlugins::setEditor(SqlTool *value)
{
    if (value) {
        ui->bt_test->setEnabled(true);
    } else {
        ui->bt_test->setEnabled(false);
    }
    sql_tool = value;
}

void DlgPlugins::on_bt_close_clicked()
{
    accept();
}

void DlgPlugins::on_bt_test_clicked()
{
    QString name;
    int row_sel = ui->plugin_table->currentRow();
    QStringList response;

    QMapIterator<QString, PluginElement*> i(interface_list);
    name = ui->plugin_table->item(row_sel,0)->text();
    for(auto i = interface_list.begin(); i != interface_list.end(); i++) {
        if(name == i.value()->getMeta().toObject().value("Name").toString()) {
            response = i.value()->getInterface()->run(0);
            foreach(QString str, response) {
                sql_tool->getCurrentEditor()->append(str);
            }
        }
    }
}

void DlgPlugins::loadTable()
{
    QTableWidgetItem *plugin_name;
    QTableWidgetItem *author;
    QTableWidgetItem *vendor;
    QTableWidgetItem *url;
    QTableWidgetItem *version;

    ui->bt_test->setEnabled(false);

    ui->plugin_table->setColumnWidth(0,120);
    ui->plugin_table->setColumnWidth(1,140);
    ui->plugin_table->setColumnWidth(2,120);
    ui->plugin_table->setColumnWidth(3,350);
    ui->plugin_table->setColumnWidth(4,120);

    QMapIterator<QString, PluginElement *> i(interface_list);
    while (i.hasNext()) {
        i.next();

        ui->plugin_table->setRowCount(ui->plugin_table->rowCount()+1);
        plugin_name = new QTableWidgetItem(i.value()->getMeta().toObject().value("Name").toString());
        author = new QTableWidgetItem(i.value()->getMeta().toObject().value("Author").toString());
        vendor = new QTableWidgetItem(i.value()->getMeta().toObject().value("Vendor").toString());
        url = new QTableWidgetItem(i.value()->getMeta().toObject().value("URL").toString());
        version = new QTableWidgetItem(i.value()->getMeta().toObject().value("Version").toString());

        ui->plugin_table->setItem(ui->plugin_table->rowCount()-1, 0, plugin_name);
        ui->plugin_table->setItem(ui->plugin_table->rowCount()-1, 1, author);
        ui->plugin_table->setItem(ui->plugin_table->rowCount()-1, 2, vendor);
        ui->plugin_table->setItem(ui->plugin_table->rowCount()-1, 3, url);
        ui->plugin_table->setItem(ui->plugin_table->rowCount()-1, 4, version);

    }
}
