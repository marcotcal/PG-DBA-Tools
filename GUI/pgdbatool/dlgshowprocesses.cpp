#include "dlgshowprocesses.h"
#include "ui_dlgshowprocesses.h"

DlgShowProcesses::DlgShowProcesses(ConnectionsData &connections, int sel_connection, QWidget *parent) :
    QDialog(parent),    
    ui(new Ui::DlgShowProcesses),
    connections(connections),
    sel_connection(sel_connection)
{
    ui->setupUi(this);
    for (int i = 0; i < connections.getConnections().count(); i++) {
        ConnectionElement *conn = connections.getConnections().at(i);
        if (!conn->isInvalid())
            ui->connection_list->addItem(conn->name());
    }

    ui->connection_list->setCurrentIndex(sel_connection);
}

DlgShowProcesses::~DlgShowProcesses()
{
    delete ui;
}

void DlgShowProcesses::on_btClose_clicked()
{
    close();
}

