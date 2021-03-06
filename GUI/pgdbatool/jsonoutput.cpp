#include "jsonoutput.h"


JSONOutput::JSONOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent) :
    ResultOutput(output, messages, parent)
{

}

void JSONOutput::generateOutput(PGresult *res)
{
    int tuples = PQntuples(res);
    int columns = PQnfields(res);
    QList<QVariant> row;
    QList<QList<QVariant>> rows;
    QList<QString> fields;

    messages->insertPlainText(QString("Number of rows returned by the last command: %1\n").arg(tuples));
    messages->insertPlainText(QString("Number of columns returned by the last command: %1\n").arg(columns));
    if (fetch_limit != -1 && fetch_limit < tuples)
        messages->insertPlainText(QString("Returned tuples where limited bt: %1 rows \n").arg(fetch_limit));

    dynamic_cast<QPlainTextEdit *>(output)->clear();

    // get field names
    for (int i = 0; i < columns; i++) {
        fields.append(QString::fromStdString(PQfname(res, i)));
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

    dynamic_cast<QPlainTextEdit *>(output)->insertPlainText("{\"rows\": [\n");
    for (int r = 0; r < rows.count(); r++) {
        dynamic_cast<QPlainTextEdit *>(output)->insertPlainText("    {\"row\": {");
        for (int c = 0; c < rows.at(r).count(); c++) {
            if(c == 0)
                dynamic_cast<QPlainTextEdit *>(output)->insertPlainText("\"");
            else
                dynamic_cast<QPlainTextEdit *>(output)->insertPlainText(", \"");
            dynamic_cast<QPlainTextEdit *>(output)->insertPlainText(fields.at(c));
            dynamic_cast<QPlainTextEdit *>(output)->insertPlainText("\":\"");
            dynamic_cast<QPlainTextEdit *>(output)->insertPlainText(rows.at(r).at(c).toString().trimmed());
            dynamic_cast<QPlainTextEdit *>(output)->insertPlainText("\"");
        }
        if(r != rows.count()-1)
            dynamic_cast<QPlainTextEdit *>(output)->insertPlainText("}},\n");
        else
            dynamic_cast<QPlainTextEdit *>(output)->insertPlainText("}}\n");
    }
    dynamic_cast<QPlainTextEdit *>(output)->insertPlainText("]}\n");

}

void JSONOutput::clearOutput()
{
    dynamic_cast<QPlainTextEdit *>(output)->clear();
}
