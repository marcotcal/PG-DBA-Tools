#ifndef SQLTOOL_H
#define SQLTOOL_H

#include <QWidget>
#include "Qsci/qsciscintilla.h"
#include <Qsci/qscilexersql.h>
#include <Qsci/qscilexer.h>

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
    explicit SqlTool(QWidget *parent = nullptr);
    ~SqlTool();
    void addEditor();
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
private:
    Ui::SqlTool *ui;
    QList<EditorItem*> editors;
    bool in_transaction;
    bool is_connected;

    void initializeEditor(EditorItem *editor);
};

#endif // SQLTOOL_H
