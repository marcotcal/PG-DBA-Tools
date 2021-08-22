#include "dlgparameters.h"
#include "ui_dlgparameters.h"
#include <QMessageBox>

DlgParameters::DlgParameters(QueryModelData *data, ConnectionsData &connections,
                             int sel_connection, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgParameters),
    model_data(data),
    connections(connections),
    sel_connection(sel_connection)
{
    ui->setupUi(this);
    ui->parameter_stack->setCurrentIndex(1);
    loadParameters();
}

DlgParameters::~DlgParameters()
{
    delete ui;
}

QString DlgParameters::getSelectedDatabaseName()
{
    return ui->database_list->itemText(ui->database_list->currentIndex());
}

void DlgParameters::loadParameters()
{
    QTableWidgetItem *item;

    if (!model_data->getDatabaseRequest()) {
         ui->database_list->setEnabled(false);
    } else {
        loadDatabases();
    }

    ui->parameter_table->clear();

    if(model_data->getParameters().count() >0) {

        ui->parameter_stack->setCurrentIndex(0);

        ui->parameter_table->setRowCount(model_data->getParameters().count());
        ui->parameter_table->setColumnCount(2);
        ui->parameter_table->setHorizontalHeaderLabels(QStringList() << "Parameter" << "Value");
        ui->parameter_table->setColumnWidth(0, 150);
        ui->parameter_table->setColumnWidth(1, 440);

        ui->parameter_table->verticalHeader()->hide();
        ui->parameter_table->horizontalHeader()->hide();

        for (int i = 0; i < model_data->getParameters().count(); i++) {
            item = new QTableWidgetItem(model_data->getParameters().at(i)->getDescription());
            item->setFlags(item->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled));
            item->setBackground(this->palette().color(QPalette::Button));
            item->setTextColor(this->palette().color(QPalette::ButtonText));
            ui->parameter_table->setItem(i, 0, item);
            ui->parameter_table->setItem(i, 1, new QTableWidgetItem(""));
        }
        ui->parameter_table->setCurrentCell(0, 1);        
    } else {
        ui->parameter_stack->setHidden(true);
        adjustSize();
    }

    for(int i = 0; i < model_data->getOrders().count(); i++)
        ui->cb_order->addItem(model_data->getOrders().at(i)->getDescription());
}

void DlgParameters::assignValues()
{
    for (int i = 0; i < ui->parameter_table->rowCount(); i++) {
        if (ui->parameter_table->item(i, 1)->text() != "") {
            model_data->getParameters().at(i)->setValue(ui->parameter_table->item(i, 1)->text());
        } else {
            model_data->getParameters().at(i)->setValue(QVariant());
        }
    }
    if (model_data->getOrders().count() > 0)
        model_data->setOrderBy(" ORDER BY " + model_data->getOrders().at(ui->cb_order->currentIndex())->getFields());
    else
        model_data->setOrderBy("");
}

void DlgParameters::loadDatabases()
{
    QMessageBox msg;
    QString conn_str;
    int tuples;
    const char *sql =
            "SELECT db.datname "
            "FROM pg_database db "
            "ORDER BY db.datname ";
    PGconn *conn;

    if (sel_connection != -1) {
        conn_str = connections.getConnections().at(sel_connection)->connectStr();

        conn = PQconnectdb(conn_str.toStdString().c_str());

        if (PQstatus(conn) == CONNECTION_OK) {
            PGresult *res = PQexec(conn, sql);

            if (PQresultStatus(res) != PGRES_TUPLES_OK)
            {
                msg.setText(QString("Read Database: %1").arg(PQerrorMessage(conn)));
                msg.exec();
                PQclear(res);
                PQfinish(conn);
                return;
            }

            tuples = PQntuples(res);

            ui->database_list->clear();

            for (int i = 0; i < tuples; i++)
                ui->database_list->addItem(QString::fromStdString(PQgetvalue(res, i, 0)));

            PQclear(res);
            PQfinish(conn);

        }
    }
}

void DlgParameters::on_buttonBox_accepted()
{
    assignValues();
}
