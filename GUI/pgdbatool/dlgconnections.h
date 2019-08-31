/***************************************************************************
                      dlgconnections.h  -  description
                      --------------------------------
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
#ifndef DLGCONNECTIONS_H
#define DLGCONNECTIONS_H
#include <libpq-fe.h>
#include <QDialog>
#include <QWidget>
#include <QListWidgetItem>
#include "connectionsdata.h"

namespace Ui {
class DlgConnections;
}

class DlgConnections : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnections(ConnectionsData &conn, QWidget *parent = nullptr);
    ~DlgConnections();
    enum Mode {EDIT_MODE, INSERT_MODE, BROWSE_MODE};
    void setEditingMode(Mode state);
private slots:
    void on_bt_close_clicked();
    void on_bt_add_connection_clicked();
    void on_bt_delete_connection_clicked();
    void on_connection_list_itemActivated(QListWidgetItem *item);
    void on_bt_sort_clicked();
    void on_connection_list_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_bt_edit_clicked();
    void on_bt_save_clicked();
    void on_bt_cancel_clicked();

private:
    Ui::DlgConnections *ui;
    PGconn *connection;
    ConnectionsData &connections;
    void connectionToEditors(int conn);
    void editorsToConnection(int conn);
    void initializeNew();
    void loadList();    
    int current_row;
    Mode mode;
};

#endif // DLGCONNECTIONS_H
