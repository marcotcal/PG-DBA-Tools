#ifndef XMLTEXTOUTPUT_H
#define XMLTEXTOUTPUT_H

#include <QObject>
#include "resultoutput.h"

class XMLTextOutput : public ResultOutput
{
    Q_OBJECT
public:
    XMLTextOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent = nullptr);
    virtual void generateOutput(PGresult *res);
signals:

public slots:
};

#endif // XMLTEXTOUTPUT_H
