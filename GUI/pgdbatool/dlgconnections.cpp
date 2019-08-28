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

DlgConnections::DlgConnections(ConnectionsData &conn, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConnections),
    connections(conn)
{
    ui->setupUi(this);
    QListWidgetItem *item;
    for (int i = 0; i < connections.getConnections().count(); i++) {
        ConnectionElement *conn = connections.getConnections().at(i);
        item = new QListWidgetItem(conn->name(), ui->connection_list);
        item->setData(Qt::UserRole, QVariant(conn->id()));
    }
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
    ui->connection_name->setText(connections.getConnections().at(conn)->name());
    ui->host->setText(connections.getConnections().at(conn)->parameter("host").toString());
    ui->port->setValue(connections.getConnections().at(conn)->parameter("port").toInt());
    ui->user_name->setText(connections.getConnections().at(conn)->parameter("user").toString());
    ui->password->setText(connections.getConnections().at(conn)->parameter("password").toString());
    ui->database->setText(connections.getConnections().at(conn)->parameter("dbname").toString());
    ui->service->setText(connections.getConnections().at(conn)->parameter("service").toString());
}
