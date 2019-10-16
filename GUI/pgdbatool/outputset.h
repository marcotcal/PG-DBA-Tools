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

private:
    Ui::OutputSet *ui;
};

#endif // OUTPUTSET_H
