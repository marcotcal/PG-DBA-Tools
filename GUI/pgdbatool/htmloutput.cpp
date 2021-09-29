#include "htmloutput.h"
#if defined HML_USE_WEBENGINE
    #include <QWebEngineView>
#elif defined HML_USE_WEBKIT
    #include <QWebView>
#else
    #include <qtextbrowser.h>
#endif


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
#ifdef HML_USE_WEBENGINE
    QWebEngineView *view = dynamic_cast<QWebEngineView *>(output);
#endif
#ifndef HML_USE_WEBENGINE
#ifdef HML_USE_WEBKIT
    QWebView *view = dynamic_cast<QWebView *>(output);
#endif
#ifndef HML_USE_WEBKIT
    QTextBrowser *view = dynamic_cast<QTextBrowser *>(output);
#endif
#endif

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

    html += "<html>";
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
            "  border-collapse: collapse;"
            "}"
            "</style>"
            "</head>"
            "<body>";

    QMap<QString, QVariant>::iterator it = getInformationMap().begin();

    if (getInformationMap().begin() != getInformationMap().end()) {
        html += "<table cellpadding='5' style='font-family:\"Courier New\", Courier, monospace; font-size:80%' border='1'>";
        html += "  <tr>";
    }

    while(it != getInformationMap().end()) {

        html += "    <th align='left'>";
        html += it.key();
        html += "    </th>";
        html += "    <td>";
        html += it.value().toString();
        html += "    </td>";
        html += "  </tr>";

        ++it;
    }

    if (getInformationMap().begin() != getInformationMap().end()) {
        html += "</table>";
        html += "<br>";
    }

    html += "<table cellpadding='5' style='font-family:\"Courier New\", Courier, monospace; font-size:80%' border='1'><tr>";

    for(int c = 0; c < fields.count(); c++) {
        html += "<th>" + fields.at(c) + "</th>";
    }
    html += "</tr>";

    for (int r = 0; r < rows.count(); r++) {
        html += "<tr>";
        for (int c = 0; c < rows.at(r).count(); c++) {
            html += "<td>" + rows.at(r).at(c).toString()
                                .replace("\n","<br>")
                                .replace(" ","&nbsp;")
                                .replace("\t","&nbsp;&nbsp;&nbsp;&nbsp;") +
                    "</td>";
        }
        html += "</tr>";
    }

    html += "</table></body></html>";

    view->setHtml(html);
}

void HtmlOutput::clearOutput()
{
#ifdef HML_USE_WEBENGINE
    QWebEngineView *view = dynamic_cast<QWebEngineView *>(output);
#endif
#ifndef HML_USE_WEBENGINE
#ifdef HML_USE_WEBKIT
    QWebView *view = dynamic_cast<QWebView *>(output);
#endif
#ifndef HML_USE_WEBKIT
    QTextBrowser *view = dynamic_cast<QTextBrowser *>(output);
#endif
#endif
    view->setHtml("<html><body></body></html>");
}
