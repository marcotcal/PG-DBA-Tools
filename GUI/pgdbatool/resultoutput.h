#ifndef RESULTOUTPUT_H
#define RESULTOUTPUT_H

#include <QObject>
#include <QWidget>
#include <QPlainTextEdit>
#include <libpq-fe.h>

class ResultOutput : public QObject
{
    Q_OBJECT
public:
    explicit ResultOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent = nullptr);
    ~ResultOutput();
    virtual void generateOutput(PGresult *res) = 0;
    virtual void generateError(PGconn *conn);
    virtual void generateStatusMessage(PGresult *res);
    void cleanMessage();
    void setFetchLimit(int value);
signals:

public slots:

protected:

    QWidget *output;
    QPlainTextEdit *messages;
    PGresult *result;
    PGconn *connection;
    QTextCursor message_cursor;
    int fetch_limit;

private:

};

#endif // RESULTOUTPUT_H
