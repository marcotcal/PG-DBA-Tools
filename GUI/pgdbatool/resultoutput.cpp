#include "resultoutput.h"

ResultOutput::ResultOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent) :
    QObject(parent),
    output(output),
    messages(messages)
{
    message_cursor = messages->textCursor();
}

ResultOutput::~ResultOutput()
{

}

void ResultOutput::generateError(PGconn *conn)
{
    messages->moveCursor(QTextCursor::End);
    messages->insertPlainText (QString("Command failed: %1").arg(PQerrorMessage(conn)));
}

void ResultOutput::generateStatusMessage(PGresult *res)
{
    messages->moveCursor(QTextCursor::End);
    messages->insertPlainText(PQresStatus(PQresultStatus(res)));
}

void ResultOutput::cleanMessage()
{
    messages->clear();
    messages->moveCursor(QTextCursor::Start);

}
