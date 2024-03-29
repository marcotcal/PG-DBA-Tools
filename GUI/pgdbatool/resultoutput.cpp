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
    clean_before = true;
}

ResultOutput::~ResultOutput()
{

}

void ResultOutput::generateError(PGconn *conn)
{
    messages->moveCursor(QTextCursor::End);
    messages->insertPlainText (QString("Command failed: %1").arg(PQerrorMessage(conn)));
}

void ResultOutput::generateListerMessage(QString event, int pid, QString payload)
{
    QString msg = QString("ASYNC NOTIFY of %1 received from backend PID %2, payload %3").arg(event).
                     arg(pid).arg(payload);
    messages->moveCursor(QTextCursor::End);
    messages->insertPlainText(msg+"\n");
}

void ResultOutput::generateStatusMessage(PGresult *res)
{
    QString msg(PQresStatus(PQresultStatus(res)));
    messages->moveCursor(QTextCursor::End);
    messages->insertPlainText(msg+"\n");
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
