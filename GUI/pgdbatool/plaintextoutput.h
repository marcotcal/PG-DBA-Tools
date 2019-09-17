#ifndef PLAINTEXTOUTPUT_H
#define PLAINTEXTOUTPUT_H

#include <QObject>
#include "resultoutput.h"

class PlainTextOutput : public ResultOutput
{
    Q_OBJECT
public:
    PlainTextOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent = nullptr);
    virtual void generateOutput(PGresult *res);
signals:

public slots:
};

#endif // PLAINTEXTOUTPUT_H
