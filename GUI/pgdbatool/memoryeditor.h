#ifndef MEMORYEDITOR_H
#define MEMORYEDITOR_H

#include <QWidget>

namespace Ui {
class MemoryEditor;
}

class MemoryEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryEditor(QWidget *parent = nullptr);
    ~MemoryEditor();
    void setValue(qlonglong size);
    qlonglong getValue();
private slots:
    void on_combo_currentIndexChanged(int index);

    void on_spinner_editingFinished();

private:

    enum mem_unit_size {
        MS_KB = 0,
        MS_MB = 1,
        MS_GB = 2,
        MS_TB = 3
    };

    struct mem_params {
        long size;
        mem_unit_size unit;
        int min;
        int max;
        int step;
    };

    Ui::MemoryEditor *ui;
};

#endif // MEMORYEDITOR_H
