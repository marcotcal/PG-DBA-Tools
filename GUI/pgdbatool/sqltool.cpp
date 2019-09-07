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
#include "sqltool.h"
#include "ui_sqltool.h"
#include "postgresqllexer.h"

EditorItem::EditorItem(QWidget *parent) : QsciScintilla (parent) {
    connect(this, SIGNAL(textChanged()), this, SLOT(setAsModified()));
}

void EditorItem::setIsModified(bool value) {
    is_modified = value;
}

void EditorItem::setAsModified() {
    is_modified = true;
}

bool EditorItem::getIsModified() {
    return is_modified;
}

EditorItem::~EditorItem() {

}

SqlTool::SqlTool(ConnectionsData &connections, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SqlTool),
    connections(connections)
{
    ui->setupUi(this);
    addEditor();
    in_transaction = false;
    is_connected = false;
    ui->connection_list->clear();
    for (int i = 0; i < connections.getConnections().count(); i++) {
        ConnectionElement *conn = connections.getConnections().at(i);
        ui->connection_list->addItem(conn->name());
    }
}

SqlTool::~SqlTool()
{
    int count = editors.count();

    delete ui;

    for (int i = 0; i < count; i++)
      delete (editors.takeAt(0));
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

bool SqlTool::openFileOnCurrent()
{
    QString file_name;
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());

    file_name = QFileDialog::getOpenFileName(this, "Open SQL File", "./", "sql files (*.sql);; All files (*.*)");

    if(file_name != "") {

        QFile file(file_name);
        if (!editor)
            editor = addEditor();

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;
        QTextStream in(&file);
        editor->setText(in.readAll());
        editor->setIsModified(false);
        return true;
    }
    return false;
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
    editor->setIsModified(false);
}

EditorItem *SqlTool::addEditor() {

    QString new_sufix = QString("%1").arg(ui->editors_tabs->count()+1);
    EditorItem *editor = new EditorItem();

    editor->setObjectName("editor"+new_sufix);
    initializeEditor(editor);
    editors.append(editor);
    editor->setObjectName("SQL "+new_sufix);
    ui->editors_tabs->addTab(editor, "SQL "+new_sufix);
    ui->editors_tabs->setCurrentIndex(ui->editors_tabs->count()-1);
    editor->setFocus();
    return editor;
}

bool SqlTool::closeCurrentEditor() {
    EditorItem *editor = dynamic_cast<EditorItem *>(ui->editors_tabs->currentWidget());
    QMessageBox msgBox;
    QString file_name;

    if (editor->getIsModified()) {
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

    ui->led_connected->setStyleSheet("background-color:#00FF00;border-radius:6;");
    is_connected = true;
}

void SqlTool::databaseDisconnect() {

    ui->led_connected->setStyleSheet("background-color:#008800;border-radius:6;");
    is_connected = false;
}

bool SqlTool::transaction() {
    return in_transaction;
}

void SqlTool::beginTransaction() {

    ui->led_transaction->setStyleSheet("background-color:#FFE000;border-radius:6;");
    in_transaction = true;
}

void SqlTool::executeCurrent() {

}

void SqlTool::rollback() {

    ui->led_transaction->setStyleSheet("background-color:#C46709;border-radius:6;");
    in_transaction = false;
}

void SqlTool::commit() {

    ui->led_transaction->setStyleSheet("background-color:#C46709;border-radius:6;");
    in_transaction = false;
}
