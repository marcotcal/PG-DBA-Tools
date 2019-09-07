/***************************************************************************
                      dlgconnections.cpp  -  description
                      ----------------------------------
    begin                : Sex Aug 23 2019
    copyright            : (C) 2003 by Marco Túlio Castro
    email                : marco.castro@gmai.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "dlgconnections.h"
#include "ui_dlgconnections.h"
#include <QtConfig>
#include <QMessageBox>
#include <algorithm>
#include <libpq-fe.h>

DlgConnections::DlgConnections(ConnectionsData &conn, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConnections),
    connections(conn)
{
    ui->setupUi(this);
    ui->configurations->setCurrentIndex(0);
    connections.sortByName();
    loadList();
    if (ui->connection_list->count() > 0)
        ui->connection_list->setCurrentRow(0);
}

DlgConnections::~DlgConnections()
{
    delete ui;
}

void DlgConnections::on_bt_close_clicked()
{
    close();
}

void DlgConnections::on_bt_add_connection_clicked()
{
    initializeNew();
    setEditingMode(INSERT_MODE);
}

void DlgConnections::on_bt_delete_connection_clicked()
{

}

void DlgConnections::on_connection_list_itemActivated(QListWidgetItem *item)
{
    connectionToEditors(ui->connection_list->row(item));
}

void DlgConnections::connectionToEditors(int conn)
{
    ConnectionElement *ele = connections.getConnections().at(conn);
    ui->connection_name->setText(ele->name());
    ui->host->setText(ele->getParameter("host").toString());
    ui->port->setValue(ele->getParameter("port").toInt());
    ui->user_name->setText(ele->getParameter("user").toString());
    ui->password->setText(ele->getParameter("password").toString());
    ui->database->setText(ele->getParameter("dbname").toString());
    ui->service->setText(ele->getParameter("service").toString());
}

void DlgConnections::editorsToConnection(int conn)
{
    ConnectionElement *ele = connections.getConnections().at(conn);
    ele->setName(ui->connection_name->text());
    ele->addParameter("host", ui->host->text());
    ele->addParameter("port", ui->port->value());
    ele->addParameter("user", ui->user_name->text());
    ele->addParameter("password", ui->password->text());
    ele->addParameter("dbname", ui->database->text());
    ele->addParameter("service", ui->service->text());
}

void DlgConnections::initializeNew()
{
    ui->connection_name->setText("New Connection");
    ui->host->setText("localhost");
    ui->port->setValue(5432);
    ui->database->setText("postgres");
    ui->user_name->setText("postgres");
    ui->password->setText("");
    ui->service->setText("");
}

void DlgConnections::loadList()
{
    ui->connection_list->clear();
    for (int i = 0; i < connections.getConnections().count(); i++) {
        ConnectionElement *conn = connections.getConnections().at(i);
        new QListWidgetItem(conn->name(), ui->connection_list);
    }
}

void DlgConnections::setEditingMode(Mode state)
{
    switch (state) {
        case EDIT_MODE:
        case INSERT_MODE:
            ui->connection_name->setEnabled(true);
            ui->configurations->setEnabled(true);
            ui->connection_list->setEnabled(false);
            ui->bt_add_connection->setEnabled(false);
            ui->bt_delete_connection->setEnabled(false);
            ui->bt_sort->setEnabled(false);
            ui->bt_edit->setEnabled(false);
            ui->bt_save->setEnabled(true);
            ui->bt_cancel->setEnabled(true);
            break;
        case BROWSE_MODE:
            ui->connection_name->setEnabled(false);
            ui->configurations->setEnabled(false);
            ui->connection_list->setEnabled(true);
            ui->bt_add_connection->setEnabled(true);
            ui->bt_delete_connection->setEnabled(true);
            ui->bt_sort->setEnabled(true);
            ui->bt_edit->setEnabled(true);
            ui->bt_save->setEnabled(false);
            ui->bt_cancel->setEnabled(false);
            break;
    }

    mode = state;
}

void DlgConnections::on_bt_sort_clicked()
{
    connections.sortByName();
    loadList();
}

void DlgConnections::on_connection_list_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous)
    if (current) {
        current_row = ui->connection_list->row(current);
        connectionToEditors(current_row);
    } else {
        current_row = -1;
    }
}

void DlgConnections::on_bt_edit_clicked()
{
    setEditingMode(EDIT_MODE);
}

void DlgConnections::on_bt_save_clicked()
{
    if (mode == EDIT_MODE) {
        editorsToConnection(current_row);        
    } else {
        current_row = ui->connection_list->count();
        ConnectionElement *conn = connections.newConnection();
        editorsToConnection(current_row);
        new QListWidgetItem(conn->name(), ui->connection_list);
        ui->connection_list->setCurrentRow(current_row);        
    }

    ui->connection_list->item(current_row)->setText(ui->connection_name->text());
    editorsToConnection(current_row);

    setEditingMode(BROWSE_MODE);

}

void DlgConnections::on_bt_cancel_clicked()
{
    setEditingMode(BROWSE_MODE);
    connectionToEditors(current_row);
}

void DlgConnections::on_bt_test_connetion_clicked()
{
    QMessageBox msg;
    PGconn *conn;
    QString conn_str = "";

    if (!ui->host->text().isEmpty())
        conn_str += "host="+ui->host->text();
    if (!ui->database->text().isEmpty())
        conn_str += " dbname="+ui->database->text();
    if (!ui->user_name->text().isEmpty())
        conn_str += " user="+ui->user_name->text();
    if (!ui->password->text().isEmpty())
        conn_str += " password="+ui->password->text();
    if (ui->port->value() > 0)
        conn_str += QString(" port=%1").arg(ui->port->value());
    if (!ui->service->text().isEmpty())
        conn_str += " service="+ui->service->text();

    // TODO - Add the other parameters to the connection

    conn = PQconnectdb(conn_str.toStdString().c_str());

    if (PQstatus(conn) == CONNECTION_OK) {
        PQfinish(conn);
        msg.setText("Successful Connection");
        msg.exec();
        return;
    }
    msg.setText(QString("Fail to Connect - %1").arg(PQerrorMessage(conn)));
    msg.exec();

}
