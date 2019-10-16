#include "resultoutput.h"

ResultOutput::ResultOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent) :
    QObject(parent),
    output(output),
    messages(messages)
{
    message_cursor = messages->textCursor();
    fetch_limit = -1;
    show_border = true;
    show_header = true;
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

void ResultOutput::generateStatusMessage(QString msg)
{
    messages->moveCursor(QTextCursor::End);
    messages->insertPlainText(msg);
}

void ResultOutput::cleanMessage()
{
    messages->clear();
    messages->moveCursor(QTextCursor::Start);

}

void ResultOutput::setFetchLimit(int value)
{
    fetch_limit = value;
}
