#include "htmloutput.h"
#include <qwebengineview.h>

HtmlOutput::HtmlOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent) :
    ResultOutput(output, messages, parent)
{

}

void HtmlOutput::generateOutput(PGresult *res)
{
    int tuples = PQntuples(res);
    int columns = PQnfields(res);
    QList<QVariant> row;
    QList<QList<QVariant>> rows;
    QList<QString> fields;
    QWebEngineView *view = dynamic_cast<QWebEngineView *>(output);
    QString html = "";

    messages->insertPlainText(QString("Number of rows returned by the last command: %1\n").arg(tuples));
    messages->insertPlainText(QString("Number of columns returned by the last command: %1\n").arg(columns));
    if (fetch_limit != -1 && fetch_limit < tuples)
        messages->insertPlainText(QString("Returned tuples where limited bt: %1 rows \n").arg(fetch_limit));

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

    html += "<html><body>";
    html += "<head>"
            "<style>"
            "table {"
            "  border-collapse: collapse;"
            "}"
            "table th {"
            "  background-color: #aaaaaa;"
            "}"
            "table, td, th {"
            "  border: 1px solid #cccccc;"
            "}"
            "</style>";

    html += "<table cellpadding='5' style='font-family:\"Courier New\", Courier, monospace; font-size:80%' border='1'><tr>";

    for(int c = 0; c < fields.count(); c++) {
        html += "<th>" + fields.at(c) + "</th>";
    }
    html += "</tr>";

    for (int r = 0; r < rows.count(); r++) {
        html += "<tr>";
        for (int c = 0; c < rows.at(r).count(); c++) {
            html += "<td>" + rows.at(r).at(c).toString() + "</td>";
        }
        html += "</tr>";
    }

    html += "</table></body></html>";

    view->page()->setHtml(html);
}

void HtmlOutput::clearOutput()
{
    QWebEngineView *view = dynamic_cast<QWebEngineView *>(output);
    view->page()->setHtml("<html><body></body></html>");
}
