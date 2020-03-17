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
#include "connectionsettings.h"
#include "projectdata.h"

namespace Ui {
class SqlTool;
}

class EditorItem : public QsciScintilla  {

    Q_OBJECT

public:
    explicit EditorItem(QWidget *parent = nullptr);
    ~EditorItem();
    void setFileName(QString value);
    const QString &getFileName() const;
    void contextMenuEvent(QContextMenuEvent *event);

public slots:

    void on_reserved_word_uppercase_triggered();
    void on_delete_selection_triggered();

private:
    QString file_name;
};

class SqlTool : public QWidget
{
    Q_OBJECT

public:
    explicit SqlTool(ConnectionsData &connections, ProjectData &project, QWidget *parent = nullptr);
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
    bool saveCurrent(bool force=false);
    bool saveCurrentAs();
    void setGroupName(const QString &value) { group_name = value; }
    QString &getGroupName() { return group_name; }
    QString getCurrentEditorName();
    bool saveGroup();
    bool saveGroupAs();
    bool restoreGroup();
    bool isRunning() { return query_running; }
    void cancelCurrentQuery();
    void plannerMethodSettings();
    void geneticQueryOptimizationsSettings();
    void parallelismSettings();
    void workersSettings();
    void otherPlanningSettings();
private slots:
    void on_limit_result_clicked(bool checked);
    void do_query_ended(PGresult *res);

    void on_ck_use_alternate_user_clicked(bool checked);

signals:
    void beginExecution(SqlTool *sender);
    void endExecution(SqlTool *sender);

private:
    Ui::SqlTool *ui;
    enum {
        st_name,
        st_setting,
        st_unit,
        st_category,
        st_short_desc,
        st_extra_desc,
        st_context,
        st_vartype,
        st_source,
        st_min_val,
        st_max_val,
        st_enumvals,
        st_boot_val,
        st_reset_val,
        st_sourcefile,
        st_sourceline,
        st_pending_restart
    };
    QList<EditorItem*> editors;
    bool in_transaction;
    bool is_connected;
    ConnectionsData &connections;
    ProjectData &project;
    PGconn *conn;
    QString default_path;
    QString group_name;
    OutputSet *output_set;
    ResultOutput *output;
    bool query_running;
    ConnectionSettings *conn_settings;

    void initializeEditor(EditorItem *editor);
    bool saveToXML(QString file_name);
    bool readFromXML(QString file_name);

};

#endif // SQLTOOL_H
