/***************************************************************************
                      sqltoll.h  -  description
                      -------------------------
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
/***************************************************************************
 * Based on the lexersql
 ***************************************************************************/
#ifndef SQLTOOL_H
#define SQLTOOL_H

#include <QWidget>
#include "Qsci/qsciscintilla.h"
#include <Qsci/qscilexersql.h>
#include <Qsci/qscilexer.h>
#include "connectionsdata.h"
#include <libpq-fe.h>

namespace Ui {
class SqlTool;
}

class EditorItem : public QsciScintilla  {

    Q_OBJECT

public:
    explicit EditorItem(QWidget *parent = nullptr);
    ~EditorItem();
    bool getIsModified();
    void setIsModified(bool value);
private slots:
    void setAsModified();
private:
    bool is_modified;
};

class SqlTool : public QWidget
{
    Q_OBJECT

public:
    explicit SqlTool(ConnectionsData &connections, QWidget *parent = nullptr);
    ~SqlTool();
    EditorItem *addEditor();
    bool closeCurrentEditor();
    bool connected();
    void databaseConnect();
    void databaseDisconnect();
    bool transaction();
    void beginTransaction();
    void executeCurrent();
    void rollback();
    void commit();
    bool closeAllEditors();
    bool openFileOnCurrent(QFile &file);
private slots:
    void on_limit_result_clicked(bool checked);

private:
    Ui::SqlTool *ui;
    QList<EditorItem*> editors;
    bool in_transaction;
    bool is_connected;
    ConnectionsData &connections;
    PGconn *conn;

    void initializeEditor(EditorItem *editor);
};

#endif // SQLTOOL_H
