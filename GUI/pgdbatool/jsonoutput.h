#ifndef JSONOUTPUT_H
#define JSONOUTPUT_H

#include <QObject>
#include "resultoutput.h"

class JSONOutput : public ResultOutput
{
    Q_OBJECT
public:
    JSONOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent = nullptr);
    virtual void generateOutput(PGresult *res);
signals:

public slots:
};

#endif // JSONOUTPUT_H
