#ifndef HTMLOUTPUT_H
#define HTMLOUTPUT_H

#include "resultoutput.h"
#include <qobjectdefs.h>

class HtmlOutput : public ResultOutput
{
    Q_OBJECT
public:
    HtmlOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent = nullptr);
    virtual void generateOutput(PGresult *res);
    virtual void clearOutput();
signals:

public slots:
};

#endif // HTMLOUTPUT_H
