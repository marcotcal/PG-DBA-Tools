#include "plaintextoutput.h"

PlainTextOutput::PlainTextOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent) :
    ResultOutput(output, messages, parent)
{

}

void PlainTextOutput::generateOutput(PGresult *res)
{
    int tuples = PQntuples(res);
    int columns = PQnfields(res);    
    QMap<int, QVariant> row;
    QList< QMap<int, QVariant>> rows;
    cleanMessage();

    messages->insertPlainText(QString("Number of rows returned by the last command: %1\n").arg(tuples));
    messages->insertPlainText(QString("Number of columns returned by the last command: %1\n").arg(columns));

    for (int i = 0; i < columns; i++) {
        messages->insertPlainText(QString("%1 - %2 - %3 - %4\n").arg(PQfname(res, i))
                                  .arg(PQftype(res,i)).arg(PQfmod(res,i)).arg(PQgetlength(res,1,i)));
    }

    dynamic_cast<QPlainTextEdit *>(output)->clear();

    /*
    for (int i = 0; i < columns; i++) {
        QString cell;
        cell.sprintf("%-15s", PQfname(res, i));
        dynamic_cast<QPlainTextEdit *>(output)->insertPlainText(cell);
    }
    dynamic_cast<QPlainTextEdit *>(output)->insertPlainText("\n");
    */


    for (int i = 0; i < tuples; i++)
    {
        for (int j = 0; j < columns; j++) {
            QVariant value = QString::fromStdString(PQgetvalue(res, i, j));
            row[i] = value;
        }
        rows.append(row);
    }

    foreach(row, rows) {
        for (QMap<int, QVariant>::iterator it = row.begin(); it != row.end(); it++ ) {
            QString cell;
            cell.sprintf("%-15s", it.value().toString());
            dynamic_cast<QPlainTextEdit *>(output)->insertPlainText(cell);
        }
    }

}
