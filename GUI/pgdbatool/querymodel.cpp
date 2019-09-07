#include "querymodel.h"
#include "ui_querymodel.h"
#include "postgresqllexer.h"
#include <QFontDatabase>
#include <QFileDialog>
#include <QTextStream>

QueryModel::QueryModel(ConnectionsData &connections, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryModel),
    connections(connections)
{
    ui->setupUi(this);
    initializeParameters();
    initializeEditor();
    ui->code->setValidator(new UpercaseValidator);    
    ui->connection_list->clear();
    for (int i = 0; i < connections.getConnections().count(); i++) {
        ConnectionElement *conn = connections.getConnections().at(i);
        ui->connection_list->addItem(conn->name());
    }
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

    file_name = QFileDialog::getOpenFileName(this, "Open Model File", "./", "Model files (*.xml);;All files (*.*)");

    if (data.loadFromFile(file_name)) {

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
            item->data(Qt::CheckStateRole);
            if (param->getMandatory())
                item->setCheckState(Qt::Checked);
            else {
                item->setCheckState(Qt::Unchecked);
            }
            ui->parameter_table->setItem(row, 2, item);
	    row++;
        }

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

void QueryModel::saveFile()
{

}
