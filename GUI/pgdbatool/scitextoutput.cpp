#include "scitextoutput.h"
#include <Qsci/qsciscintilla.h>

SciTextOutput::SciTextOutput(QWidget *output, QPlainTextEdit *messages, QObject *parent) :
    ResultOutput(output, messages, parent)
{

    QsciScintilla *editor = dynamic_cast<QsciScintilla* >(output);
    QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);

    fixedFont.setPointSize(12);

    editor->setFont(fixedFont);
    editor->setMarginsBackgroundColor(QColor(100,100,100,255));
    editor->setMarginLineNumbers(0, true);
    editor->setMarginWidth(0, 30);
    editor->setTabWidth(4);
    editor->setTabIndents(false);
    editor->setIndentationsUseTabs(false);
    editor->setIndentationGuides(true);
    editor->setAutoIndent(true);

}

void SciTextOutput::generateOutput(PGresult *res)
{
    int tuples = PQntuples(res);
    int columns = PQnfields(res);
    QString line_separator = "";
    QList<QVariant> row;
    QList<QList<QVariant>> rows;
    QList<QString> fields;
    QList<int> max_field_lengths;
    QsciScintilla *editor = dynamic_cast<QsciScintilla* >(output);

    messages->insertPlainText(QString("Number of rows returned by the last command: %1\n").arg(tuples));
    messages->insertPlainText(QString("Number of columns returned by the last command: %1\n").arg(columns));
    if (fetch_limit != -1 && fetch_limit < tuples)
        messages->insertPlainText(QString("Returned tuples where limited bt: %1 rows \n").arg(fetch_limit));

    editor->clear();

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

    if (show_header) {
        editor->append(line_separator);
        editor->append("|");
        for(int c = 0; c < fields.count(); c++) {
            editor->append(
                        fields.at(c).leftJustified(max_field_lengths.at(c)) + "|");
        }
        editor->append("\n");
    }

    if (show_border) {
        editor->append(line_separator);
    }

    for (int r = 0; r < rows.count(); r++) {
        editor->append(show_border ? "|" : " ");
        for (int c = 0; c < rows.at(r).count(); c++) {
            editor->append(
                        rows.at(r).at(c).toString()
                        .leftJustified(max_field_lengths.at(c)) + (show_border ? "|" : " "));
        }
        editor->append("\n");
    }

    if (show_border) {
        editor->append(line_separator);
    }
}

void SciTextOutput::clearOutput()
{
    dynamic_cast<QsciScintilla* >(output)->clear();
}
