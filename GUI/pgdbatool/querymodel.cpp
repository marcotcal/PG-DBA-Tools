#include "querymodel.h"
#include "ui_querymodel.h"
#include "postgresqllexer.h"
#include <QFontDatabase>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

QueryModel::QueryModel(ConnectionsData &connections, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryModel),
    connections(connections)
{
    ui->setupUi(this);
    initializeParameters();
    initializeOrders();
    initializeColumns();
    initializeEditor();
    ui->code->setValidator(new UpercaseValidator);    
    ui->connection_list->clear();
    for (int i = 0; i < connections.getConnections().count(); i++) {
        ConnectionElement *conn = connections.getConnections().at(i);
        ui->connection_list->addItem(conn->name());
    }
    ui->splitter->setStretchFactor(1,4);
}

QueryModel::~QueryModel()
{
    delete ui;
}

void QueryModel::initializeEditor() {
    PostgreSQLLexer *lex =new PostgreSQLLexer(this);
    QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    fixedFont.setPointSize(12);

    lex->setFont(fixedFont);
    ui->sql_editor->setMarginsBackgroundColor(QColor(100,100,100,255));
    ui->sql_editor->setMarginLineNumbers(0, true);
    ui->sql_editor->setMarginWidth(0, 30);
    ui->sql_editor->setTabWidth(4);
    ui->sql_editor->setTabIndents(false);
    ui->sql_editor->setIndentationsUseTabs(false);
    ui->sql_editor->setIndentationGuides(true);
    ui->sql_editor->setAutoIndent(true);
    ui->sql_editor->setLexer(lex);
    is_modified = false;
}

bool QueryModel::openFile()
{
    QString file_name;    
    QTableWidgetItem *item;
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    QString path = settings.value("path_to_models", "").toString();

    file_name = QFileDialog::getOpenFileName(this, "Open Model File", path, "Model files (*.xml);;All files (*.*)");

    if (data.loadFromFile(file_name)) {

        ui->code->clear();
        ui->description->clear();
        ui->sql_editor->clear();
        ui->parameter_table->clear();
        ui->order_table->clear();
        ui->column_table->clear();

        ui->code->setText(data.getCode());
        ui->description->setText(data.getDescription());
        ui->sql_editor->setText(data.getQueryText());

        ui->parameter_table->setRowCount(data.getParameters().count());

        int row = 0;
        foreach(QueryParameter *param, data.getParameters()) {
            item = new QTableWidgetItem(param->getCode());
            ui->parameter_table->setItem(row, 0, item);
            item = new QTableWidgetItem(param->getDescription());
            ui->parameter_table->setItem(row, 1, item);
            item = new QTableWidgetItem();
            item->setFlags(item->flags() & ~ Qt::ItemIsEditable);
            item->data(Qt::CheckStateRole);
            if (param->getMandatory())
                item->setCheckState(Qt::Checked);
            else {
                item->setCheckState(Qt::Unchecked);
            }
            ui->parameter_table->setItem(row, 2, item);
            row++;
        }

        ui->order_table->setRowCount(data.getOrders().count());

        row = 0;
        foreach(QueryOrder *order, data.getOrders()) {
            item = new QTableWidgetItem(order->getDescription());
            ui->order_table->setItem(row, 0, item);
            item = new QTableWidgetItem(order->getFields());
            ui->order_table->setItem(row, 1, item);
            row++;
        }

        ui->column_table->setRowCount(data.getColumns().count());

        row = 0;
        foreach(QueryColumn *column, data.getColumns()) {            
            item = new QTableWidgetItem(column->getTitle());
            ui->column_table->setItem(row, 0, item);
            item = new QTableWidgetItem(QString("%1").arg(column->getWidth()));
            ui->column_table->setItem(row, 1, item);
            row++;
        }

        is_modified = false;

        return true;
    } else
        return false;
}

void QueryModel::initializeParameters()
{
    ui->parameter_table->setColumnWidth(0, 100);
    ui->parameter_table->setColumnWidth(1, 500);
    ui->parameter_table->setColumnWidth(2, 100);
}

void QueryModel::initializeOrders()
{
    ui->order_table->setColumnWidth(0,250);
    ui->order_table->setColumnWidth(1,150);
}

void QueryModel::initializeColumns()
{
    ui->column_table->setColumnWidth(0,155);
    ui->column_table->setColumnWidth(1,60);
}

void QueryModel::saveFile()
{

}

void QueryModel::on_bt_add_parameter_clicked()
{
    int last_row = 0;
    QTableWidgetItem *item;

    last_row = ui->parameter_table->rowCount();
    ui->parameter_table->setRowCount(ui->parameter_table->rowCount()+1);

    item = new QTableWidgetItem("");
    ui->parameter_table->setItem(last_row, 0, item);

    item = new QTableWidgetItem("");
    ui->parameter_table->setItem(last_row, 1, item);

    item = new QTableWidgetItem();
    item->data(Qt::CheckStateRole);
    item->setCheckState(Qt::Unchecked);
    item->setFlags(item->flags() & ~ Qt::ItemIsEditable);    
    ui->parameter_table->setItem(last_row, 2, item);

    is_modified = true;
}

void QueryModel::on_bt_delete_parameter_clicked()
{
    ui->parameter_table->removeRow(ui->parameter_table->currentRow());

    is_modified = true;
}

void QueryModel::on_bt_test_parameter_clicked()
{

}

void QueryModel::on_bt_add_order_clicked()
{
    int last_row = 0;
    QTableWidgetItem *item;

    last_row = ui->order_table->rowCount();
    ui->order_table->setRowCount(ui->order_table->rowCount()+1);

    item = new QTableWidgetItem("");
    ui->order_table->setItem(last_row, 0, item);

    item = new QTableWidgetItem("");
    ui->order_table->setItem(last_row, 1, item);

    is_modified = true;
}

void QueryModel::on_bt_delete_order_clicked()
{
    ui->order_table->removeRow(ui->order_table->currentRow());
}

void QueryModel::on_bt_order_up_clicked()
{
    QString sel_col1;
    QString sel_col2;
    QString pre_col1;
    QString pre_col2;
    int sel_row = ui->order_table->currentRow();

    if(sel_row > 0) {

        sel_col1 = ui->order_table->item(sel_row, 0)->text();
        sel_col2 = ui->order_table->item(sel_row, 1)->text();
        pre_col1 = ui->order_table->item(sel_row -1, 0)->text();
        pre_col2 = ui->order_table->item(sel_row -1, 1)->text();

        ui->order_table->setItem(sel_row, 0, new QTableWidgetItem(pre_col1));
        ui->order_table->setItem(sel_row, 1, new QTableWidgetItem(pre_col2));
        ui->order_table->setItem(sel_row -1, 0, new QTableWidgetItem(sel_col1));
        ui->order_table->setItem(sel_row -1, 1, new QTableWidgetItem(sel_col2));

        ui->order_table->selectRow(sel_row-1);

        is_modified = true;
    }
}

void QueryModel::on_bt_order_down_clicked()
{
    QString sel_col1;
    QString sel_col2;
    QString pos_col1;
    QString pos_col2;
    int sel_row = ui->order_table->currentRow();

    if(sel_row < ui->order_table->rowCount()-1) {

        sel_col1 = ui->order_table->item(sel_row, 0)->text();
        sel_col2 = ui->order_table->item(sel_row, 1)->text();
        pos_col1 = ui->order_table->item(sel_row + 1, 0)->text();
        pos_col2 = ui->order_table->item(sel_row + 1, 1)->text();

        ui->order_table->setItem(sel_row, 0, new QTableWidgetItem(pos_col1));
        ui->order_table->setItem(sel_row, 1, new QTableWidgetItem(pos_col2));
        ui->order_table->setItem(sel_row + 1, 0, new QTableWidgetItem(sel_col1));
        ui->order_table->setItem(sel_row + 1, 1, new QTableWidgetItem(sel_col2));

        ui->order_table->selectRow(sel_row+1);

        is_modified = true;
    }
}

void QueryModel::on_bt_add_column_clicked()
{
    int last_row = 0;
    QTableWidgetItem *item;

    last_row = ui->column_table->rowCount();
    ui->column_table->setRowCount(ui->column_table->rowCount()+1);

    item = new QTableWidgetItem(QString("column_%1").arg(last_row+1));
    ui->column_table->setItem(last_row, 0, item);

    item = new QTableWidgetItem("200");
    ui->column_table->setItem(last_row, 1, item);

    is_modified = true;
}

void QueryModel::on_bt_delete_column_clicked()
{
    ui->column_table->removeRow(ui->column_table->currentRow());

    is_modified = true;
}

void QueryModel::on_bt_column_up_clicked()
{
    QString sel_col1;
    QString sel_col2;
    QString pre_col1;
    QString pre_col2;
    int sel_row = ui->column_table->currentRow();

    if(sel_row > 0) {

        sel_col1 = ui->column_table->item(sel_row, 0)->text();
        sel_col2 = ui->column_table->item(sel_row, 1)->text();
        pre_col1 = ui->column_table->item(sel_row -1, 0)->text();
        pre_col2 = ui->column_table->item(sel_row -1, 1)->text();

        ui->column_table->setItem(sel_row, 0, new QTableWidgetItem(pre_col1));
        ui->column_table->setItem(sel_row, 1, new QTableWidgetItem(pre_col2));
        ui->column_table->setItem(sel_row -1, 0, new QTableWidgetItem(sel_col1));
        ui->column_table->setItem(sel_row -1, 1, new QTableWidgetItem(sel_col2));

        ui->column_table->selectRow(sel_row-1);

        is_modified = true;
    }    
}

void QueryModel::on_bt_column_down_clicked()
{
    QString sel_col1;
    QString sel_col2;
    QString pos_col1;
    QString pos_col2;
    int sel_row = ui->column_table->currentRow();

    if(sel_row < ui->column_table->rowCount()-1) {

        sel_col1 = ui->column_table->item(sel_row, 0)->text();
        sel_col2 = ui->column_table->item(sel_row, 1)->text();
        pos_col1 = ui->column_table->item(sel_row + 1, 0)->text();
        pos_col2 = ui->column_table->item(sel_row + 1, 1)->text();

        ui->column_table->setItem(sel_row, 0, new QTableWidgetItem(pos_col1));
        ui->column_table->setItem(sel_row, 1, new QTableWidgetItem(pos_col2));
        ui->column_table->setItem(sel_row + 1, 0, new QTableWidgetItem(sel_col1));
        ui->column_table->setItem(sel_row + 1, 1, new QTableWidgetItem(sel_col2));

        ui->column_table->selectRow(sel_row+1);

        is_modified = true;
    }
}

void QueryModel::on_bt_test_query_clicked()
{

}

bool QueryModel::canCloseOrReplace()
{
    QMessageBox msgBox;
    QString file_name;

    if (is_modified) {
        msgBox.setText(QString("The query model has been modified."));
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch(ret) {
            case QMessageBox::Save:
                file_name = QFileDialog::getSaveFileName(this, "Save SQL File", "./", "sql files (*.sql);;All files (*.*)");
                if (file_name != "") {
                    if (!data.saveModel())
                        return false;
                } else {
                    return false;
                }
                break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return false;
            default:
                break;
        }
    }

    return true;
}

void QueryModel::on_sql_editor_textChanged()
{
    is_modified = true;
}

void QueryModel::on_parameter_table_itemChanged(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    is_modified = true;
}

void QueryModel::on_order_table_itemChanged(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    is_modified = true;
}

void QueryModel::on_column_table_itemChanged(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    is_modified = true;
}
