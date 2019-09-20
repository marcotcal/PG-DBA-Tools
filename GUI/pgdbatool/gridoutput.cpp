#include "gridoutput.h"
#include <QTableWidget>

GridOutput::GridOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent) :
    ResultOutput(output, messages, parent)
{

}

void GridOutput::generateOutput(PGresult *res)
{
    int tuples = PQntuples(res);
    int columns = PQnfields(res);
    QList<QVariant> row;
    QList<QList<QVariant>> rows;
    QList<QString> fields;
    QList<int> max_field_lengths;
    QTableWidget *table;
    QStringList titles;

    table = dynamic_cast<QTableWidget *>(output);

    cleanMessage();

    messages->insertPlainText(QString("Number of rows returned by the last command: %1\n").arg(tuples));
    messages->insertPlainText(QString("Number of columns returned by the last command: %1\n").arg(columns));
    table->setColumnCount(columns);
    if (fetch_limit != -1 && fetch_limit < tuples) {
        messages->insertPlainText(QString("Returned tuples where limited bt: %1 rows \n").arg(fetch_limit));
        table->setRowCount(fetch_limit);
    } else {
        table->setRowCount(tuples);
    }

    QFontMetrics fm(table->font());

    // get field names
    for (int i = 0; i < columns; i++) {
        fields.append(QString::fromStdString(PQfname(res, i)));
        max_field_lengths.append(fm.width(QString::fromStdString(PQfname(res, i))));
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
            if (max_field_lengths.at(c) < fm.width(rows.at(r).at(c).toString()))
                max_field_lengths.replace(c, fm.width(rows.at(r).at(c).toString()));
        }
    }

    for(int c = 0; c < fields.count(); c++) {
        titles << fields.at(c);
        table->setColumnWidth(c, 40 + max_field_lengths.at(c));
    }

    table->setHorizontalHeaderLabels(titles);

    for (int r = 0; r < rows.count(); r++) {

        for (int c = 0; c < rows.at(r).count(); c++) {
            table->setItem(r, c, new QTableWidgetItem(rows.at(r).at(c).toString()));
        }

    }

}
