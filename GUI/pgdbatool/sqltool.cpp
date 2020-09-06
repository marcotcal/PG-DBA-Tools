/***************************************************************************
                      sqltoll.cpp  -  description
                      ----------------------------
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
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QRegularExpression>
#include <QFileInfo>
#include <QFile>
#include <QMenu>
#include <QAction>
#include <QFontDatabase>
#include <QTextEdit>
#include <QClipboard>
#include <QMimeData>
#include "sqltool.h"
#include "ui_sqltool.h"
#include "postgresqllexer.h"
#include "queryexecutor.h"
#include "dlgplanmethods.h"
#include "dlggqoconfig.h"
#include "dlgparallelsettings.h"
#include "dlgworkers.h"
#include "dlgotherplanningsettings.h"

//  libraries

#include "pgdbagenerators.h"

EditorItem::EditorItem(QWidget *parent) : QsciScintilla (parent) {
    file_name = "";    
    signal_mapper = new QSignalMapper(this);
    connect(signal_mapper, SIGNAL(mapped(int)), this, SLOT(do_execute_generator(int)));
}

void EditorItem::setFileName(QString value)
{
    file_name = value;
}

const QString &EditorItem::getFileName() const
{
    return file_name;
}

void EditorItem::contextMenuEvent(QContextMenuEvent *event)
{
    //QMenu *menu = createStandardContextMenu();
    QMenu *menu = new QMenu();
    QAction *action;
    // parent chain - stack, tab, sqltool
    SqlTool *editor = dynamic_cast<SqlTool *>(parent()->parent()->parent());
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    bool connected = editor->isConnected();

    QMenu *mddl = new QMenu(tr("SQL Generation"));
    QMenu *msql = new QMenu(tr("DDL Generation"));

    action = menu->addAction(tr("Find..."));
    action->setShortcut(Qt::CTRL+'f');
    connect(action, SIGNAL(triggered()), this, SLOT(find_triggered()));
    action->setEnabled(!text().isEmpty());

    menu->addSeparator();

    action = menu->addAction(tr("Undo"));
    action->setShortcut(Qt::CTRL+'z');
    connect(action, SIGNAL(triggered()), this, SLOT(undo()));
    action->setEnabled(isUndoAvailable());

    action = menu->addAction(tr("Redo"));
    action->setShortcut(Qt::CTRL+'y');
    connect(action, SIGNAL(triggered()), this, SLOT(redo()));
    action->setEnabled(isRedoAvailable());

    menu->addSeparator();

    action = menu->addAction(tr("Cut"));
    action->setShortcut(Qt::CTRL+'x');
    connect(action, SIGNAL(triggered()), this, SLOT(cut()));
    action->setEnabled(hasSelectedText());

    action = menu->addAction(tr("Copy"));
    action->setShortcut(Qt::CTRL+'c');
    connect(action, SIGNAL(triggered()), this, SLOT(copy()));
    action->setEnabled(hasSelectedText());

    action = menu->addAction(tr("Paste"));
    action->setShortcut(Qt::CTRL+'v');
    connect(action, SIGNAL(triggered()), this, SLOT(paste()));
    action->setEnabled((mimeData->hasHtml() || mimeData->hasText()));

    action = menu->addAction(tr("Delete"));
    action->setEnabled(hasSelectedText());
    connect(action, SIGNAL(triggered()), this, SLOT(on_delete_selection_triggered()));

    menu->addSeparator();

    action = menu->addAction(tr("Select All"));
    action->setShortcut(Qt::CTRL+'a');
    connect(action, SIGNAL(triggered()), this, SLOT(selectAll()));
    action->setEnabled(!text().isEmpty());

    menu->addSeparator();

    action = menu->addAction(tr("Reserved Word to Uppercase"));
    action->setEnabled(hasSelectedText());
    connect(action, SIGNAL(triggered()), this, SLOT(on_reserved_word_uppercase_triggered()));

    menu->addSeparator();

    menu->addMenu(msql);
    menu->addMenu(mddl);

    action = msql->addAction(tr("Generate Insert (All fields)"));
    action->setEnabled(connected);
    connect(action, SIGNAL(triggered()), signal_mapper, SLOT(map()));
    signal_mapper->setMapping(action, GEN_INSERT_ALL);

    action = msql->addAction(tr("Generate Insert (Only Mandatory)"));
    action->setEnabled(connected);
    connect(action, SIGNAL(triggered()), signal_mapper, SLOT(map()));
    signal_mapper->setMapping(action, GEN_INSERT_MANDATORY);

    action = msql->addAction(tr("Generate Update (All fields)"));
    action->setEnabled(connected);
    connect(action, SIGNAL(triggered()), signal_mapper, SLOT(map()));
    signal_mapper->setMapping(action, GEN_UPDATE_ALL);

    action = msql->addAction(tr("Generate Update (Only Mandatory)"));
    action->setEnabled(connected);
    connect(action, SIGNAL(triggered()), signal_mapper, SLOT(map()));
    signal_mapper->setMapping(action, GEN_UPDATE_MANDATORY);

    menu->exec(event->globalPos());

    delete menu;

}

void EditorItem::on_reserved_word_uppercase_triggered()
{
    QStringList keywords = QString(lexer()->keywords(1)).split(" ");
    QString text;  

    if (hasSelectedText()) {
        text = selectedText();
        foreach (QString word, keywords)  {
            QRegExp re("\\b("+word+")\\b");
            re.setCaseSensitivity(Qt::CaseInsensitive);
            text = text.replace(re, word.toUpper());
        }
        replaceSelectedText(text);
    }
}

void EditorItem::on_delete_selection_triggered()
{
    if (hasSelectedText()) {
        replaceSelectedText("");
    }
}

void EditorItem::find_triggered()
{
    emit find_request(this);
}

void EditorItem::do_execute_generator(int gen_sql)
{
    emit execute_generator(this, gen_sql);
}

EditorItem::~EditorItem() {

}

SqlTool::SqlTool(ConnectionsData &connections, int sel_connection, ProjectData &project, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SqlTool),
    connections(connections), 
    project(project),
    sel_connection(sel_connection)
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    ui->setupUi(this);
    addEditor();
    in_transaction = false;
    is_connected = false;
    ui->database_list->clear();
    loadDatabaseList(sel_connection);

    for (int i = 0; i < connections.getConnections().count(); i++) {
        ConnectionElement *conn = connections.getConnections().at(i);
        if (!conn->isInvalid())
            ui->connection_list->addItem(conn->name());
    }

    ui->connection_list->setCurrentIndex(sel_connection);

    if(project.getProjectName().isNull()) {
        default_path = settings.value("path_to_sql", "").toString();
    } else {
        default_path = project.getProjectPath() + "/scripts/development/review";
    }

    group_name = "";
    query_running = false;
    conn_settings = nullptr;
    ui->find_panel->hide();

    use_find_next = true;
    last_backward = true;
    last_forward = true;

    mode = MODE_QUERY;

    //connect(ui->text_to_find, SIGNAL(editingFinished()), this, SLOT(on_modify_find_control()));
    //connect(ui->from_line, SIGNAL(editingFinished()), this, SLOT(on_modify_find_control()));
    connect(ui->from_cursor, SIGNAL(pressed()), this, SLOT(do_modify_find_control()));
    connect(ui->case_sensitive, SIGNAL(pressed()), this, SLOT(do_modify_find_control()));
    connect(ui->regular_expression, SIGNAL(pressed()), this, SLOT(do_modify_find_control()));
    connect(ui->posix_regular_expression, SIGNAL(pressed()), this, SLOT(do_modify_find_control()));
    connect(ui->whole_word, SIGNAL(pressed()), this, SLOT(do_modify_find_control()));
    connect(ui->warp_around, SIGNAL(pressed()), this, SLOT(do_modify_find_control()));

    last_search_path = "public,pg_catalog";
}

SqlTool::~SqlTool()
{
    int count = editors.count();
    if (query_running)
        cancelCurrentQuery();

    delete ui;

    for (int i = 0; i < count; i++)
        delete (editors.takeAt(0));

    if (is_connected)
        databaseDisconnect();
}

bool SqlTool::closeAllEditors() {

    int count;

    count = editors.count();
    for(int i = 0; i < count; i++) {
        ui->editors_tabs->setCurrentIndex(i);
        if(!closeCurrentEditor())
            return false;
    }

    return true;
}

bool SqlTool::openFileOnCurrent(QFile &file)
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());

    if (!editor)
        editor = addEditor();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    editor->setFileName(file.fileName());
    editor->setText(in.readAll());
    editor->setModified(false);
    ui->editors_tabs->tabBar()->setTabText(ui->editors_tabs->currentIndex(),
                                           QFileInfo(file).baseName());

    return true;
}

bool SqlTool::saveCurrent(bool force)
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        if (editor->isModified() || force) {
            if (editor->getFileName() != "") {
                QFile destination(editor->getFileName());
                if(destination.open(QIODevice::WriteOnly)) {
                    QTextStream ts(&destination);
                    ts << editor->text();
                    editor->setModified(false);
                    ui->editors_tabs->setTabText(ui->editors_tabs->currentIndex(),QFileInfo(editor->getFileName()).baseName());
                    return true;
                }
            } else {                
                return saveCurrentAs();
            }
        }
    }
    return false;
}

bool SqlTool::saveCurrentAs()
{
    QString file_name;
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        file_name = QFileDialog::getSaveFileName(this, "Save SQL File", default_path, "sql files (*.sql);;All files (*.*)");
        if (file_name != "") {
            editor->setFileName(file_name);
            return saveCurrent(true); // cannot set modifyed to true at QsciScintilla class
        }
    }
    return false;
}

QString SqlTool::getCurrentEditorName()
{
    return ui->editors_tabs->tabText(ui->editors_tabs->currentIndex()).replace("&","");
}

EditorItem *SqlTool::getCurrentEditor()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->widget(ui->editors_tabs->currentIndex()));
    return editor;
}

bool SqlTool::saveGroup()
{
    if (group_name == "")
        return saveGroupAs();
    return saveToXML(group_name);
}

bool SqlTool::saveGroupAs()
{
    QString file_name;

    file_name = QFileDialog::getSaveFileName(this, "Save Query Tool As", default_path, "Query Tool files (*.qtx);;All files (*.*)");

    if (file_name != "") {
        return saveToXML(file_name);
    }
    return false;
}

bool SqlTool::restoreGroup()
{
    QString file_name;

    file_name = QFileDialog::getOpenFileName(this, "Open File", default_path, "Query Tool files (*.qtx);;All files (*.*))");

    if (file_name != "") {
        closeAllEditors();
        return readFromXML(file_name);
    }
    return false;
}

void SqlTool::cancelCurrentQuery()
{
    PGcancel *cancel = PQgetCancel(conn);
    char error[256];
    QMessageBox msg;

    if (!PQcancel(cancel, error, 256)) {
        msg.setText(QString("Cancel failed: %1").arg(error));
        msg.exec();
    }
}

void SqlTool::plannerMethodSettings()
{
    DlgPlanMethods dlg(conn_settings);

    if(dlg.exec()) {


    }
}

void SqlTool::geneticQueryOptimizationsSettings()
{
    DlgGQOConfig dlg(conn_settings);

    if(dlg.exec()) {

    }
}

void SqlTool::parallelismSettings()
{
    DlgParallelSettings dlg(conn_settings);

    if(dlg.exec()) {

    }
}

void SqlTool::workersSettings()
{
    DlgWorkers dlg;

    if(dlg.exec()) {

    }
}

void SqlTool::otherPlanningSettings()
{
    DlgOtherPlanningSettings dlg;

    if(dlg.exec()) {

    }
}

int SqlTool::getSelectedConnectionIndex()
{
    return ui->connection_list->currentIndex();
}

bool SqlTool::isRedoAvailable()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        return editor->isRedoAvailable();
    }
    return false;
}

bool SqlTool::isUndoAvailable()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        return editor->isUndoAvailable();
    }
    return false;
}

bool SqlTool::isCutAvailable()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        return editor->hasSelectedText();
    }
    return false;
}

bool SqlTool::isCopyAvailable()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        return editor->hasSelectedText();
    }
    return false;
}

bool SqlTool::isPasteAvailabe()
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        return (mimeData->hasHtml() || mimeData->hasText());
    }
    return false;
}

bool SqlTool::isDeleteAvailable()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        return editor->hasSelectedText();
    }
    return false;
}

bool SqlTool::isSelectAllAvailable()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        return !editor->text().isEmpty();
    }
    return false;
}

bool SqlTool::isFindAvailable()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        return !editor->text().isEmpty();
    }
    return false;
}

bool SqlTool::isFindNextAvailable()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        return use_find_next;
    }
    return false;
}

bool SqlTool::isConnected()
{
    return is_connected;
}

void SqlTool::redo()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        editor->redo();
    }
}

void SqlTool::undo()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        editor->undo();
    }
}

void SqlTool::cut()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        editor->cut();
    }
}

void SqlTool::copy()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        editor->copy();
    }
}

void SqlTool::paste()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        editor->paste();
    }
}

void SqlTool::deleteText()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        editor->replaceSelectedText("");
    }
}

void SqlTool::selectAll()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        editor->selectAll();
    }
}

void SqlTool::findFirst()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        ui->text_to_find->setFocus();
        ui->find_panel->show();        
    }
}

void SqlTool::findNext()
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {
        editor->findNext();
    }
}

int SqlTool::getMode() {
    return mode;
}

void SqlTool::setMode(int value)
{
    mode = value;
    switch(mode) {
    case MODE_INTERNAL_DEVELOPMENT:
    case MODE_INTERNAL_STAGING:
    case MODE_INTERNAL_PRODUCTION:
        ui->config_frame->setVisible(false);
        break;
    default:
        ui->config_frame->setVisible(true);
    }
}

void SqlTool::initializeEditor(EditorItem *editor) {
    PostgreSQLLexer *lex =new PostgreSQLLexer(this);
    QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    fixedFont.setPointSize(12);

    lex->setFont(fixedFont);
    editor->setMarginsBackgroundColor(QColor(100,100,100,255));
    editor->setMarginLineNumbers(0, true);
    editor->setMarginWidth(0, 30);    
    editor->setTabWidth(4);
    editor->setTabIndents(false);
    editor->setIndentationsUseTabs(false);
    editor->setIndentationGuides(true);
    editor->setAutoIndent(true);
    editor->setLexer(lex);
    ui->led_connected->setStyleSheet("background-color:#008800;border-radius:6;");
    ui->led_transaction->setStyleSheet("background-color:#C46709;border-radius:6;");
    editor->setModified(false);

}

bool SqlTool::saveToXML(QString file_name)
{
    QFile file(file_name);

    group_name = QFileInfo(file).baseName();

    QString DTD =
            "<!DOCTYPE query_set [\n"
            "  <!ELEMENT query_set (queries)>\n"
            "  <!ATTLIST query_set count CDATA \"\">\n"
            "  <!ATTLIST query_set group_name CDATA \"\">\n"
            "  <!ELEMENT queries (query*)>\n"
            "  <!ELEMENT query (#PCDATA)>"
            "  <!ATTLIST query name CDATA \"\">\n"
            "]>\n";
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeDTD(DTD);

    xmlWriter.writeStartElement("query_set");
    xmlWriter.writeAttribute("","count", QString("%1").arg(editors.count()));
    xmlWriter.writeAttribute("","group_name", group_name);
    Q_FOREACH(EditorItem *editor, editors) {
        xmlWriter.writeStartElement("query");
        xmlWriter.writeAttribute("","name", QFileInfo(editor->getFileName()).baseName());
        xmlWriter.writeCDATA(editor->text());
        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();

    file.close();

    return true;
}

bool SqlTool::readFromXML(QString file_name)
{
    QFile file(file_name);
    QXmlStreamReader reader;
    QString name;
    QString query;
    EditorItem *editor;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    reader.setDevice(&file);

    while(!reader.atEnd()) {

        while(reader.readNext()) {

            if(reader.isStartDocument())
                continue;

            if (reader.isEndDocument())
                break;

            if (reader.isStartElement()) {

                if (reader.name() == "query_set") {
                    QXmlStreamAttributes attributes = reader.attributes();
                    //if(attributes.hasAttribute("count"))
                    //     count = attributes.value("count").toInt();
                    if(attributes.hasAttribute("group_name"))
                         group_name = attributes.value("group_name").toString();
                } else if (reader.name() == "query") {
                    QXmlStreamAttributes attributes = reader.attributes();
                    if(attributes.hasAttribute("name"))
                         name = attributes.value("name").toString();
                    query = reader.readElementText().trimmed();
                    editor = addEditor();
                    editor->setText(query);
                    editor->setModified(false);
                    if(name == "")
                        name = QString("SQL %1").arg(ui->editors_tabs->currentIndex() + 1 );
                    ui->editors_tabs->tabBar()->setTabText(ui->editors_tabs->currentIndex(), name);
                }
            }
        }
    }

    reader.clear();
    return true;
}

EditorItem *SqlTool::addEditor() {

    QString new_sufix = QString("%1").arg(ui->editors_tabs->count()+1);
    EditorItem *editor = new EditorItem(this);

    editor->setObjectName("editor"+new_sufix);
    initializeEditor(editor);
    editors.append(editor);
    editor->setObjectName("SQL "+new_sufix);
    ui->editors_tabs->addTab(editor, "SQL "+new_sufix);
    ui->editors_tabs->setCurrentIndex(ui->editors_tabs->count()-1);
    editor->setFocus();
    connect(editor, SIGNAL(find_request(EditorItem*)), this, SLOT(do_find_request(EditorItem*)));
    connect(editor, SIGNAL(execute_generator(EditorItem*,int)), this, SLOT(do_execute_generator(EditorItem*,int)));
    return editor;
}

bool SqlTool::closeCurrentEditor() {
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    QMessageBox msgBox;
    QString file_name;

    if (editor->isModified()) {
        msgBox.setText(QString("The document %1 has been modified.").arg(editor->objectName()));
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch(ret) {
            case QMessageBox::Save:
                file_name = QFileDialog::getSaveFileName(this, "Save SQL File", "./", "sql files (*.sql);;All files (*.*)");
                if (file_name != "") {
                    QFile destination(file_name);
                    if(destination.open(QIODevice::WriteOnly)) {
                        QTextStream ts(&destination);
                        ts << editor->text();
                    }
                } else {
                    return false;
                }
                break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return false;
            default:
                break;
        }
    }

    ui->editors_tabs->removeTab(ui->editors_tabs->currentIndex());
    editors.removeOne(editor);
    delete editor;
    return true;
}

bool SqlTool::connected() {
    return is_connected;
}

void SqlTool::databaseConnect() {
    QMessageBox msg;
    QString conn_str;
    QString database_name;
    QStringList schemas;
    int connection_index;

    switch(mode) {
    case MODE_INTERNAL_DEVELOPMENT:
            connection_index = connections.getConnectionIndexByName(project.getDevelopment());
            database_name = project.getDevelopmentDatabase();
            break;
    case MODE_INTERNAL_STAGING:
            connection_index = connections.getConnectionIndexByName(project.getStaging());
            database_name = project.getStagingDatabase();
            break;
    case MODE_INTERNAL_PRODUCTION:
            connection_index = connections.getConnectionIndexByName(project.getProduction());
            database_name = project.getProductionDatabase();
            break;
    case MODE_QUERY:
    case MODE_SCRIPT:
        database_name = ui->database_list->itemText(ui->database_list->currentIndex());
        connection_index = ui->connection_list->currentIndex();
    }

    if (!ui->ck_use_alternate_user->isChecked()) {

        if (connection_index != -1)
            conn_str = connections.getConnections().at(
                        connection_index)->connectStr("", "", database_name);
        else {
            msg.setText("No connection available. Please create a connection.");
            msg.exec();
            return;
        }

    } else {
        conn_str = connections.getConnections().at(connection_index)
                ->connectStr(ui->ed_user->text(), ui->ed_password->text(), database_name);
    }

    conn = PQconnectdb(conn_str.toStdString().c_str());

    if (PQstatus(conn) == CONNECTION_OK) {
        PQexec(conn, QString("SET application_name=\"PGDBATool - %1\"").arg(group_name).toStdString().c_str());
        ui->database_list->setEnabled(false);
        ui->connection_list->setEnabled(false);
        ui->led_connected->setStyleSheet("background-color:#00FF00;border-radius:6;");
        is_connected = true;

        ui->ck_use_alternate_user->setEnabled(false);
        ui->ed_user->setEnabled(false);
        ui->ed_password->setEnabled(false);
        ui->lb_password->setEnabled(false);

        conn_settings = new ConnectionSettings(conn, this);

        ui->schema_list->clear();

        schemas = connections.getConnections().at(sel_connection)->getSchemaList(database_name);

        ui->schema_list->addItems(schemas);
        ui->schema_list->setEnabled(true);

        return;
    }
    msg.setText(QString("Fail to Connect - %1").arg(PQerrorMessage(conn)));
    msg.exec();
}

void SqlTool::databaseDisconnect() {

    delete conn_settings;
    PQfinish(conn);
    ui->database_list->setEnabled(true);
    ui->connection_list->setEnabled(true);
    ui->led_connected->setStyleSheet("background-color:#008800;border-radius:6;");
    ui->led_transaction->setStyleSheet("background-color:#C46709;border-radius:6;");

    ui->ck_use_alternate_user->setEnabled(true);
    if (ui->ck_use_alternate_user->isChecked()) {
        ui->ed_user->setEnabled(true);
        ui->ed_password->setEnabled(true);
        ui->lb_password->setEnabled(true);
    }
    ui->schema_list->setEnabled(false);
    is_connected = false;
}

bool SqlTool::transaction() {
    return in_transaction;
}

void SqlTool::beginTransaction(QString command) {
    QMessageBox msg;

    PGresult *res = PQexec(conn, command.toStdString().c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        msg.setText(QString("BEGIN command failed: %1").arg(PQerrorMessage(conn)));
        msg.exec();
        PQclear(res);
        return;
    }

    ui->led_transaction->setStyleSheet("background-color:#FFE000;border-radius:6;");
    in_transaction = true;
}

void SqlTool::executeCurrent(ResultOutput* output, bool show_query)
{
    executeCurrent(output, "", show_query);
}

void SqlTool::executeCurrent(ResultOutput *output, QString explain, bool show_query)
{
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    QString query = !explain.isEmpty() ? explain + "\n" + editor->text() : editor->text();;
    QString search_path;
    QList<QListWidgetItem *> schemas = ui->schema_list->selectedItems();

    if (schemas.count() == 0) {
        search_path = "public,pg_catalog";
    } else {
        search_path = "";
        foreach(QListWidgetItem *item, schemas)
            search_path += item->text() + ",";
        search_path += "pg_catalog";
    }

    if (search_path != last_search_path) {
        query = "SET search_path=" + search_path + ";\n" + query;
        last_search_path = search_path;
    }

    this->output = output;

    if (ui->limit_result->isChecked())
        output->setFetchLimit(ui->line_limit->value());

    output->cleanMessage();

    if (show_query)
        output->generateStatusMessage(query+"\n");

    // execute query

    if (mode == MODE_SCRIPT) {

        if (PQtransactionStatus(conn) == PQTRANS_INERROR) {
            PGresult *res = PQexec(conn, "ROLLBACK");
            PQclear(res);
        }
    }

    query_running = true;
    ui->editors_tabs->setEnabled(false);

    QueryExecutor *executor = new QueryExecutor(conn, query);
    connect(executor, SIGNAL(finished()), executor, SLOT(deleteLater()));
    connect(executor, SIGNAL(query_ended(PGresult *)), this, SLOT(do_query_ended(PGresult *)));
    connect(executor, SIGNAL(generate_notice(QString)), output, SLOT(generateStatusMessage(QString)));

    executor->start();
    emit beginExecution(this);
}

void SqlTool::rollback() {

    QMessageBox msg;

    PGresult *res = PQexec(conn, "ROLLBACK");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        msg.setText(QString("ROLLBACK command failed: %1").arg(PQerrorMessage(conn)));
        msg.exec();
        PQclear(res);
        return;
    }

    ui->led_transaction->setStyleSheet("background-color:#C46709;border-radius:6;");
    in_transaction = false;
}

void SqlTool::commit() {
    QMessageBox msg;

    PGresult *res = PQexec(conn, "COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        msg.setText(QString("COMMIT command failed: %1").arg(PQerrorMessage(conn)));
        msg.exec();
        PQclear(res);
        return;
    }

    ui->led_transaction->setStyleSheet("background-color:#C46709;border-radius:6;");
    in_transaction = false;
}

void SqlTool::on_limit_result_clicked(bool checked)
{
    ui->line_limit->setEnabled(checked);
}

void SqlTool::do_query_ended(PGresult *res)
{    
    bool run_with_error  = false;
    QString run_dir;
    QString review_dir;
    QString done_dir;
    QString error_dir;
    QString next_run;
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    QString original_file_name = editor->getFileName();
    QString file_name = QFileInfo(original_file_name).fileName();

    switch(PQresultStatus(res)) {

    case PGRES_EMPTY_QUERY:
        output->clearOutput();
        output->generateStatusMessage(res);
        break;
    case PGRES_COMMAND_OK:
        output->clearOutput();
        output->generateStatusMessage(res);
        break;
    case PGRES_TUPLES_OK:
        output->clearOutput();
        output->generateOutput(res);
        break;
    case PGRES_SINGLE_TUPLE:

        break;
    case PGRES_COPY_IN:

        break;
    case PGRES_COPY_OUT:

        break;
    case PGRES_COPY_BOTH:

        break;
    case PGRES_BAD_RESPONSE:
        output->clearOutput();
        output->generateError(conn);
        output->generateStatusMessage(res);
        run_with_error = true;
        break;
    case PGRES_FATAL_ERROR:
        output->clearOutput();
        output->generateError(conn);
        output->generateStatusMessage(res);
        run_with_error = true;
        break;
    case PGRES_NONFATAL_ERROR:
        output->clearOutput();
        output->generateError(conn);
        output->generateStatusMessage(res);
        run_with_error = true;
        break;
    }

    PQclear(res);
    query_running = false;
    ui->editors_tabs->setEnabled(true);
    emit endExecution(this);

    if (mode == MODE_INTERNAL_DEVELOPMENT) {
        run_dir = project.getProjectPath() + "/scripts/development/run/";
        review_dir = project.getProjectPath() + "/scripts/development/review/";
        done_dir = project.getProjectPath() + "/scripts/development/done/";
        error_dir = project.getProjectPath() + "/scripts/development/error/";
        next_run = project.getProjectPath() + "/scripts/staging/run/";
        if (!run_with_error) {
            QFile::copy(original_file_name, next_run + file_name);
            QFile::rename(original_file_name, done_dir + file_name);
            emit requestToClose();
            return;
        }

        QFile::rename(original_file_name, review_dir + file_name);

        QString error = QString("Command failed: %1").arg(PQerrorMessage(conn));
        QString filename = error_dir + QDateTime::currentDateTime().toString("error_yyyyMMddhhmmsszzz") + ".txt";
        QFile file( filename );
        if ( file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &file );
            stream << error << endl;
        }
        file.close();

        emit requestToClose();

    } else if (mode == MODE_INTERNAL_STAGING) {
        run_dir = project.getProjectPath() + "/scripts/stage/run/";
        review_dir = project.getProjectPath() + "/scripts/stage/review/";
        done_dir = project.getProjectPath() + "/scripts/stage/done/";
        error_dir = project.getProjectPath() + "/scripts/stage/error/";
        next_run = project.getProjectPath() + "/scripts/production/run/";
        if (!run_with_error) {
            QFile::copy(original_file_name, next_run + file_name);
            QFile::rename(original_file_name, done_dir + file_name);
            emit requestToClose();
            return;
        }

        QFile::rename(original_file_name, review_dir + file_name);

        QString error = QString("Command failed: %1").arg(PQerrorMessage(conn));
        QString filename = error_dir + QDateTime::currentDateTime().toString("error_yyyyMMddhhmmsszzz") + ".txt";
        QFile file( filename );
        if ( file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &file );
            stream << error << endl;
        }
        file.close();

        emit requestToClose();

    } else if (mode == MODE_INTERNAL_PRODUCTION) {
        run_dir = project.getProjectPath() + "/scripts/production/run/";
        review_dir = project.getProjectPath() + "/scripts/production/review/";
        done_dir = project.getProjectPath() + "/scripts/production/done/";
        error_dir = project.getProjectPath() + "/scripts/production/error/";
        if (!run_with_error) {
            QFile::rename(original_file_name, done_dir + file_name);
            emit requestToClose();
            return;
        }

        QFile::rename(original_file_name, review_dir + file_name);

        QString error = QString("Command failed: %1").arg(PQerrorMessage(conn));
        QString filename = error_dir + QDateTime::currentDateTime().toString("error_yyyyMMddhhmmsszzz") + ".txt";
        QFile file( filename );
        if ( file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &file );
            stream << error << endl;
        }
        file.close();

        emit requestToClose();

    }
}

void SqlTool::on_ck_use_alternate_user_clicked(bool checked)
{
    ui->ed_user->setEnabled(checked);
    ui->ed_password->setEnabled(checked);
    ui->lb_password->setEnabled(checked);
    if (!checked) {
        ui->ed_user->setText("");
        ui->ed_password->setText("");
    }
}

void SqlTool::on_from_cursor_toggled(bool checked)
{
      ui->lb_from_line->setEnabled(!checked);
      ui->from_line->setEnabled(!checked);
}

void SqlTool::on_find_forward_clicked()
{
    QMessageBox msg;
    bool success;
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {

        if (last_backward) {
            use_find_next = false;
        }

        last_forward = true;
        last_backward = false;

        if (!use_find_next) {
            use_find_next = true;
            success = editor->findFirst(ui->text_to_find->text(),
                              ui->regular_expression->isChecked(),
                              ui->case_sensitive->isChecked(),
                              ui->whole_word->isChecked(),
                              ui->warp_around->isChecked(),
                              true,
                              -1,
                              -1,
                              true, // text found will be always visible
                              ui->posix_regular_expression->isChecked());
        } else {
            success = editor->findNext();
        }

        if (!success) {
            msg.setText(QString("There are no more occurrencies of <%1>").arg(ui->text_to_find->text()));
            msg.exec();
        }

    }
}

void SqlTool::on_find_backward_clicked()
{
    QMessageBox msg;
    bool success;
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    if (editor) {

        if (last_forward)
            use_find_next = false;

        last_forward = false;
        last_backward = true;

        if (!use_find_next) {
            use_find_next = true;
            ui->text_to_find->setModified(false);
            success = editor->findFirst(ui->text_to_find->text(),
                              ui->regular_expression->isChecked(),
                              ui->case_sensitive->isChecked(),
                              ui->whole_word->isChecked(),
                              ui->warp_around->isChecked(),
                              false,
                              -1,
                              -1,
                              true, // text found will be always visible
                              ui->posix_regular_expression->isChecked());
        } else {
            success = editor->findNext();
        }

        if (!success) {
            msg.setText(QString("There are no more occurrencies of <%1>").arg(ui->text_to_find->text()));
            msg.exec();
        }

    }
}

void SqlTool::do_find_request(EditorItem *editor)
{
    if (editor) {
        ui->find_panel->show();
        ui->text_to_find->setFocus();
    }
}

void SqlTool::on_close_find_clicked()
{
    ui->find_panel->hide();
}

void SqlTool::do_modify_find_control()
{
    use_find_next = false;
}

void SqlTool::on_text_to_find_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    use_find_next = false;
}

void SqlTool::on_from_line_valueChanged(const QString &arg1)
{
    Q_UNUSED(arg1)
    use_find_next = false;
}

void SqlTool::loadDatabaseList(int sel_connection)
{
    QStringList databases;

    if (sel_connection != -1) {
        databases = connections.getConnections().at(sel_connection)->getDatabaseList();

        ui->database_list->clear();
        ui->database_list->addItems(databases);
    }
}

void SqlTool::on_query_model_clicked()
{
    mode = MODE_QUERY;
    emit modeChanged(this, mode);
}

void SqlTool::on_script_mode_clicked()
{
    mode = MODE_SCRIPT;
    emit modeChanged(this, mode);
}

void SqlTool::on_connection_list_activated(int index)
{
    loadDatabaseList(index);
}

void SqlTool::do_execute_generator(EditorItem *editor, int gen_sql)
{
    PGDBAGenerators gen;

    switch(gen_sql) {

    case EditorItem::GEN_INSERT_ALL:
        gen.getInsert(conn);
        gen.setOnlyMandatory(false);
        break;

    case EditorItem::GEN_INSERT_MANDATORY:
        gen.getInsert(conn);
        gen.setOnlyMandatory(true);
        break;

    default:
        break;
    }
}
