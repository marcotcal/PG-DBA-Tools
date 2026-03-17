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
//#include <algorithm>
#include <libpq-fe.h>

DlgConnections::DlgConnections(ConnectionsData &conn, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgConnections),
    connections(conn)
{
    ui->setupUi(this);
    current_row = -1;
    ui->configurations->setCurrentIndex(0);
    connections.sortByName();
    loadList();
    if (ui->connection_list->count() > 0)
        ui->connection_list->setCurrentRow(0);

#ifndef USE_SSH_TUNNELS
    delete ui->tb_ssh_tunnel;
#endif

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
    int row = ui->connection_list->currentRow();
    QListWidgetItem *item = ui->connection_list->takeItem(row);
    delete item;
    ConnectionElement *data = connections.getConnections().takeAt(row);
    delete data;

    if (ui->connection_list->count() > 0)
        ui->connection_list->setCurrentRow(0);
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
    ui->no_ssl->setChecked(ele->getParameter("no_ssl").toBool());
    ui->require->setChecked(ele->getParameter("require").toBool());
    ui->prefer->setChecked(ele->getParameter("prefer").toBool());
    ui->allow->setChecked(ele->getParameter("allow").toBool());
    ui->disable->setChecked(ele->getParameter("disable").toBool());
    ui->verify_ca->setChecked(ele->getParameter("verify_ca").toBool());
    ui->verify_full->setChecked(ele->getParameter("verify_full").toBool());
}

void DlgConnections::editorsToConnection(int conn)
{
    ConnectionElement *ele = connections.getConnections().at(conn);
    ele->setName(ui->connection_name->text());
    if(!ui->host->text().isEmpty()) {
        ele->addParameter("host", ui->host->text());
    } else {
        ele->addParameter("host", QVariant());
    }
    ele->addParameter("port", ui->port->value());
    if(!ui->user_name->text().isEmpty()) {
        ele->addParameter("user", ui->user_name->text());
    } else {
        ele->addParameter("user", QVariant());
    }
    if (!ui->password->text().isEmpty()) {
        ele->addParameter("password", ui->password->text());
    } else {
        ele->addParameter("password", QVariant());
    }
    if (!ui->database->text().isEmpty()) {
        ele->addParameter("dbname", ui->database->text());
    } else {
        ele->addParameter("dbname", QVariant());
    }
    if (!ui->service->text().isEmpty()) {
        ele->addParameter("service", ui->service->text());
    } else {
        ele->addParameter("service", QVariant());
    }
    if (ui->no_ssl->isChecked()) {
        ele->addParameter("no_ssl", true);
    } else {
        ele->addParameter("no_ssl", false);
    }
    if (ui->allow->isChecked()) {
        ele->addParameter("allow", true);
    } else {
        ele->addParameter("allow", false);
    }
    if (ui->require->isChecked()) {
        ele->addParameter("require", true);
    } else {
        ele->addParameter("require", false);
    }
    if (ui->prefer->isChecked()) {
        ele->addParameter("prefer", true);
    } else {
        ele->addParameter("prefer", false);
    }
    if (ui->disable->isChecked()) {
        ele->addParameter("disable", true);
    } else {
        ele->addParameter("disable", false);
    }
    if (ui->verify_ca->isChecked()) {
        ele->addParameter("verify_ca", true);
    } else {
        ele->addParameter("verify_ca", false);
    }
    if (ui->verify_full->isChecked()) {
        ele->addParameter("verify_full", true);
    } else {
        ele->addParameter("verify_full", false);
    }
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
    ui->no_ssl->setChecked(true);
    ui->require->setChecked(false);
    ui->prefer->setChecked(false);
    ui->allow->setChecked(false);
    ui->disable->setChecked(false);
    ui->verify_ca->setChecked(false);
    ui->verify_full->setChecked(false);
}

void DlgConnections::loadList()
{
    ui->connection_list->clear();
    for (int i = 0; i < connections.getConnections().count(); i++) {
        ConnectionElement *conn = connections.getConnections().at(i);
        new QListWidgetItem(conn->name(), ui->connection_list);
    }
}

bool DlgConnections::testConnection()
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

    if (ui->allow->isChecked())
        conn_str += " sslmode=allow";
    else if (ui->disable->isChecked())
        conn_str += " sslmode=disable";
    else if (ui->prefer->isChecked())
        conn_str += " sslmode=prefer";
    else if (ui->require->isChecked())
        conn_str += " sslmode=require";
    else if (ui->verify_ca->isChecked())
        conn_str += " sslmode=verify-ca";
    else if (ui->verify_full->isChecked())
        conn_str += " sslmode=verify-full";

    // TODO - Add the other parameters to the connection

    conn = PQconnectdb(conn_str.toStdString().c_str());

    if (PQstatus(conn) == CONNECTION_OK) {
        PQfinish(conn);
        if (current_row != -1)
            connections.getConnections().at(current_row)->setInvalid(false);
        msg.setText("Successful Connection");
        msg.exec();
        return true;
    }
    if (current_row != -1)
        connections.getConnections().at(current_row)->setInvalid(true);
    msg.setText(QString("Fail to Connect - %1").arg(PQerrorMessage(conn)));
    msg.exec();
    return false;
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
        testConnection();
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
    testConnection();
}
