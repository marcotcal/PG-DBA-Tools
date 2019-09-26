#ifndef SCITEXTOUTPUT_H
#define SCITEXTOUTPUT_H

#include <QObject>
#include "resultoutput.h"

class SciTextOutput : public ResultOutput
{
    Q_OBJECT
public:
    SciTextOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent = nullptr);
    virtual void generateOutput(PGresult *res);
    virtual void clearOutput();
signals:

public slots:
};


#endif // SCITEXTOUTPUT_H
