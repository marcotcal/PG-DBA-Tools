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

namespace Ui {
class DlgConnections;
}

class DlgConnections : public QDialog
{
    Q_OBJECT

public:
    explicit DlgConnections(QWidget *parent = 0);
    ~DlgConnections();

private slots:
    void on_bt_close_clicked();
    void on_bt_add_connection_clicked();
    void on_bt_delete_connection_clicked();

private:
    Ui::DlgConnections *ui;
    PGconn *connection;
};

#endif // DLGCONNECTIONS_H
