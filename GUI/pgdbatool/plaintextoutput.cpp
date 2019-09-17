#include "plaintextoutput.h"

PlainTextOutput::PlainTextOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent) :
    ResultOutput(output, messages, parent)
{

}

void PlainTextOutput::generateOutput(PGresult *res)
{
    cleanMessage();
    int tuples = PQntuples(res);
    int columns = PQnfields(res);

    messages->insertPlainText(QString("Number of rows returned by the last command: %1").arg(tuples));
    messages->insertPlainText(QString("Number of columns returned by the last command: %1\n").arg(columns));

    for (int i = 0; i < columns; i++) {
        messages->insertPlainText(QString("%1 - %2\n").arg(PQfname(res, i)).arg(PQftype(res,i)));
    }

}
