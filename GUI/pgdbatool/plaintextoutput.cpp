#include "plaintextoutput.h"

PlainTextOutput::PlainTextOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent) :
    ResultOutput(output, messages, parent)
{

}

void PlainTextOutput::generateOutput(PGresult *res)
{
    int tuples = PQntuples(res);
    int columns = PQnfields(res);    
    QString line_separator = "";
    QList<QVariant> row;
    QList<QList<QVariant>> rows;
    QList<QString> fields;
    QList<int> max_field_lengths;
    cleanMessage();

    messages->insertPlainText(QString("Number of rows returned by the last command: %1\n").arg(tuples));
    messages->insertPlainText(QString("Number of columns returned by the last command: %1\n").arg(columns));
    if (fetch_limit != -1 && fetch_limit < tuples)
        messages->insertPlainText(QString("Returned tuples where limited bt: %1 rows \n").arg(fetch_limit));

    dynamic_cast<QPlainTextEdit *>(output)->clear();

    // get field names
    for (int i = 0; i < columns; i++) {
        fields.append(QString::fromStdString(PQfname(res, i)));
        max_field_lengths.append(QString::fromStdString(PQfname(res, i)).length());
    }

    // get values
    for (int i = 0; i < (fetch_limit != -1 ? fetch_limit : tuples) && i < tuples; i++)
    {        
        row.clear();
        for (int j = 0; j < columns; j++) {            
            QVariant value = QString::fromStdString(PQgetvalue(res, i, j));
            row.append(value);
        }
        rows.append(row);
    }

    for (int c = 0; c < fields.count(); c++) {
        for(int r = 0; r < rows.count(); r++) {
            if (max_field_lengths.at(c) < rows.at(r).at(c).toString().length())
                max_field_lengths.replace(c, rows.at(r).at(c).toString().length());
        }
    }

    line_separator += "+";
    for(int c = 0; c < fields.count(); c++) {
        QString rep = "-";
        line_separator += rep.repeated(max_field_lengths.at(c))+"+";
    }
    line_separator += "\n";

    dynamic_cast<QPlainTextEdit *>(output)->insertPlainText(line_separator);
    dynamic_cast<QPlainTextEdit *>(output)->insertPlainText("|");
    for(int c = 0; c < fields.count(); c++) {
        dynamic_cast<QPlainTextEdit *>(output)->insertPlainText(
                    fields.at(c).leftJustified(max_field_lengths.at(c)) + "|");
    }
    dynamic_cast<QPlainTextEdit *>(output)->insertPlainText("\n");
    dynamic_cast<QPlainTextEdit *>(output)->insertPlainText(line_separator);

    for (int r = 0; r < rows.count(); r++) {
        dynamic_cast<QPlainTextEdit *>(output)->insertPlainText("|");
        for (int c = 0; c < rows.at(r).count(); c++) {
            dynamic_cast<QPlainTextEdit *>(output)->insertPlainText(
                        rows.at(r).at(c).toString()
                        .leftJustified(max_field_lengths.at(c)) + "|");
        }
        dynamic_cast<QPlainTextEdit *>(output)->insertPlainText("\n");
    }
    dynamic_cast<QPlainTextEdit *>(output)->insertPlainText(line_separator);

}
