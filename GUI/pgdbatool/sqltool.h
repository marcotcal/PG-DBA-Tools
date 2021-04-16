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
#include "frmfindtext.h"
#include <QTreeWidget>
#include <QTabWidget>
#include <QListWidget>


namespace Ui {
class SqlTool;
}

class SqlTool;

class PluginTabWidget;

class PluginElement;

class EditorItem : public QsciScintilla  {

    Q_OBJECT

public:
    enum {
        GEN_INSERT_ALL,
        GEN_INSERT_MANDATORY,
        GEN_UPDATE_ALL,
        GEN_UPDATE_MANDATORY
    };
    explicit EditorItem(QWidget *parent = nullptr);
    ~EditorItem();
    void setFileName(QString value);
    const QString &getFileName() const;
    QString getLabel() const;
    void setLabel(const QString &value);
    void contextMenuEvent(QContextMenuEvent *event);

public slots:

    void on_reserved_word_uppercase_triggered();
    void on_delete_selection_triggered();

    void find_triggered();
    void do_execute_generator(int gen_sql);

signals:

    void find_request(EditorItem *editor);
    void execute_generator(EditorItem *editor, int gen_sql);

private: 

    QString file_name;
    QString label;

    FrmFindText *frm_find;

    QSignalMapper *signal_mapper;

};

class SqlTool : public QWidget
{
    Q_OBJECT

public:
    enum {
        MODE_QUERY,
        MODE_SCRIPT,
        MODE_INTERNAL_DEVELOPMENT,
        MODE_INTERNAL_STAGING,
        MODE_INTERNAL_PRODUCTION
    };

    explicit SqlTool(ConnectionsData &connections, int sel_connection, ProjectData &project, QWidget *parent = nullptr);
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

    void setInterfaceList(QMap<QString, PluginElement *> *list);
    QMap<QString, PluginElement *> *getInterfaceList();

    void setPluginWidgetTree(PluginTabWidget *value);
    void setFunctionList(QListWidget *value);

    PluginTabWidget *getPluginWidgetTree();
    QListWidget *getFunctionList();

    void executeCurrent(ResultOutput* output, bool show_query=false);
    void executeCurrent(ResultOutput* output, QString explain = "", bool show_query=false);
    void rollback();
    void commit();
    bool closeAllEditors();
    bool openFileOnNew(QFile &file);
    bool openFileOnCurrent(QFile &file);
    bool saveCurrent(bool force=false);
    bool saveCurrentAs();
    void setGroupName(const QString &value) { group_name = value; }
    QString &getGroupName() { return group_name; }
    QString getCurrentEditorName();
    EditorItem *getCurrentEditor();
    bool saveGroup();
    bool saveGroupAs();
    bool restoreGroup();
    bool restoreGroup(QString group_fn);
    bool isRunning() { return query_running; }
    void cancelCurrentQuery();
    void plannerMethodSettings();
    void geneticQueryOptimizationsSettings();
    void parallelismSettings();
    void workersSettings();
    void otherPlanningSettings();

    int getSelectedConnectionIndex();

    PGconn *getPostgresConnection();

    // Methods related to menu edit
    // All this methods will get info from the active editor

    bool isRedoAvailable();
    bool isUndoAvailable();
    bool isCutAvailable();
    bool isCopyAvailable();
    bool isPasteAvailabe();
    bool isDeleteAvailable();
    bool isSelectAllAvailable();
    bool isFindAvailable();
    bool isFindNextAvailable();
    bool isConnected();
    bool isModified();

    void redo();
    void undo();
    void cut();
    void copy();
    void paste();
    void deleteText();
    void selectAll();
    void findFirst();
    void findNext();

    int getMode();
    void setMode(int value);

public slots:
    void do_find_request(EditorItem *editor);
    void do_modify_find_control();

private slots:
    void on_limit_result_clicked(bool checked);
    void do_query_ended(PGresult *res);
    void on_ck_use_alternate_user_clicked(bool checked);
    void on_from_cursor_toggled(bool checked);
    void on_find_forward_clicked();
    void on_find_backward_clicked();
    void on_close_find_clicked();
    void on_text_to_find_textChanged(const QString &arg1);
    void on_from_line_valueChanged(const QString &arg1);

    void loadDatabaseList(int sel_connection);

    void on_query_model_clicked();
    void on_script_mode_clicked();
    void on_connection_list_activated(int index);
    void do_execute_generator(EditorItem *editor, int gen_sql);
    void do_execute_plugin_function(QListWidgetItem *item);

signals:
    void beginExecution(SqlTool *sender);
    void endExecution(SqlTool *sender);
    void modeChanged(SqlTool *sender, int current_mode);
    void requestToClose();
    void runPlugin(EditorItem *editor, int item);

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
    int last_editor_number;

    ConnectionsData &connections;
    ProjectData &project;
    PGconn *conn;
    QString default_path;
    QString group_name;
    QString group_file_name;

    OutputSet *output_set;

    PluginTabWidget *plugin_widget_tree;
    QListWidget *function_list;

    QMap<QString, PluginElement *> *interface_list;

    ResultOutput *output;
    bool query_running;
    ConnectionSettings *conn_settings;

    bool use_find_next;
    bool last_forward;
    bool last_backward;
    QString last_search_path;

    int mode;
    bool group_modified;

    int sel_connection;
    void initializeEditor(EditorItem *editor);
    bool saveToXML(QString file_name);
    bool readFromXML(QString file_name);

};

#endif // SQLTOOL_H
