#ifndef OUTPUTSET_H
#define OUTPUTSET_H

#include <QWidget>

namespace Ui {
class OutputSet;
}

class OutputSet : public QWidget
{
    Q_OBJECT

public:
    enum OutputType {
        OP_TEXT,
        OP_JSON,
        OP_XML,
        OP_GRID,
        OP_HTML
    };
    explicit OutputSet(QWidget *parent = nullptr);
    ~OutputSet();
    QWidget *getOutput();
    OutputType getType();

private slots:
    void on_bt_txt_clicked();
    void on_bt_json_clicked();
    void on_bt_xml_clicked();
    void on_bt_grid_clicked();
    void on_bt_html_clicked();

private:
    Ui::OutputSet *ui;
    OutputType type;
};

#endif // OUTPUTSET_H
