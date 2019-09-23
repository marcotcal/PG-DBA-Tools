#ifndef GRIDOUTPUT_H
#define GRIDOUTPUT_H

#include <QObject>
#include "resultoutput.h"

class GridOutput : public ResultOutput
{
    Q_OBJECT
public:
    GridOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent = nullptr);
    virtual void generateOutput(PGresult *res);
    virtual void clearOutput();
signals:

public slots:
};

#endif // GRIDOUTPUT_H
