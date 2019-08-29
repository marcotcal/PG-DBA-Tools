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
#include <algorithm>

DlgConnections::DlgConnections(ConnectionsData &conn, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConnections),
    connections(conn)
{
    ui->setupUi(this);
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
    ConnectionElement *conn = connections.newConnection();
    new QListWidgetItem(conn->name(), ui->connection_list);
    ui->connection_list->setCurrentRow(ui->connection_list->count()-1);
    connectionToEditors(ui->connection_list->count()-1);
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
    ui->host->setText(ele->parameter("host").toString());
    ui->port->setValue(ele->parameter("port").toInt());
    ui->user_name->setText(ele->parameter("user").toString());
    ui->password->setText(ele->parameter("password").toString());
    ui->database->setText(ele->parameter("dbname").toString());
    ui->service->setText(ele->parameter("service").toString());
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

void DlgConnections::loadList()
{
    ui->connection_list->clear();
    for (int i = 0; i < connections.getConnections().count(); i++) {
        ConnectionElement *conn = connections.getConnections().at(i);
        new QListWidgetItem(conn->name(), ui->connection_list);
    }
}

void DlgConnections::on_bt_sort_clicked()
{
    connections.sortByName();
    loadList();
}

void DlgConnections::on_connection_list_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (previous)
        editorsToConnection(ui->connection_list->row(previous));
    if (current)
        connectionToEditors(ui->connection_list->row(current));
}
