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
    explicit OutputSet(QWidget *parent = nullptr);
    ~OutputSet();

private slots:
    void on_bt_txt_clicked();

    void on_bt_json_clicked();

    void on_bt_xml_clicked();

    void on_bt_grid_clicked();

    void on_bt_html_clicked();

private:
    Ui::OutputSet *ui;
};

#endif // OUTPUTSET_H
