#ifndef RESULTOUTPUT_H
#define RESULTOUTPUT_H

#include <QObject>
#include <QWidget>
#include <QPlainTextEdit>
#include <libpq-fe.h>
#include <QList>
#include <QMap>

class ResultOutput : public QObject
{
    Q_OBJECT
public:
    explicit ResultOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent = nullptr);
    ~ResultOutput();
    void showHeader(bool value) { show_header = value; }
    void showBorder(bool value) { show_border = value; }
    virtual void generateOutput(PGresult *res) = 0;    
    virtual void clearOutput() = 0;
    void cleanMessage();
    void setFetchLimit(int value);
    QMap<QString, QVariant> &getInformationMap() { return information_map; }

signals:

public slots:

    virtual void generateStatusMessage(PGresult *res);
    virtual void generateStatusMessage(QString msg);
    virtual void generateError(PGconn *conn);

protected:

    QWidget *output;
    QPlainTextEdit *messages;
    PGresult *result;
    PGconn *connection;
    QTextCursor message_cursor;
    int fetch_limit;
    bool show_border;
    bool show_header;
    QMap<QString, QVariant> information_map;

private:

};

#endif // RESULTOUTPUT_H
