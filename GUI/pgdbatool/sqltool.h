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
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexersql.h>
#include <Qsci/qscilexer.h>
#include <libpq-fe.h>
#include "connectionsdata.h"
#include "resultoutput.h"
#include "outputset.h"

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
    void setFileName(QString value);
    const QString &getFileName() const;
private slots:
    void setAsModified();
private:
    bool is_modified;
    QString file_name;
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
    void beginTransaction(QString command);
    void setOutputSet(OutputSet *value) { output_set = value; }
    OutputSet *getOutputSet() { return output_set; }
    void executeCurrent(ResultOutput* output, bool show_query=false);
    void executeCurrent(ResultOutput* output, QString explain = "", bool show_query=false);
    void rollback();
    void commit();
    bool closeAllEditors();
    bool openFileOnCurrent(QFile &file);
    bool saveCurrent();
    bool saveCurrentAs();
    void setGroupName(const QString &value) { group_name = value; }
    QString &getGroupName() { return group_name; }
    QString getCurrentEditorName();
    bool saveGroup();
    bool saveGroupAs();
    bool restoreGroup();
    bool isRunning() { return query_running; }
private slots:
    void on_limit_result_clicked(bool checked);
    void do_query_ended(PGresult *res);

signals:
    void beginExecution(SqlTool *sender);
    void endExecution(SqlTool *sender);

private:
    Ui::SqlTool *ui;
    QList<EditorItem*> editors;
    bool in_transaction;
    bool is_connected;
    ConnectionsData &connections;
    PGconn *conn;
    QString default_path;
    QString group_name;
    OutputSet *output_set;
    ResultOutput *output;
    bool query_running;

    void initializeEditor(EditorItem *editor);
    bool saveToXML(QString file_name);
    bool readFromXML(QString file_name);

    static void noticeProcessor(void *arg, const char *message);
};

#endif // SQLTOOL_H
