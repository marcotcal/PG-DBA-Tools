#ifndef QUERYMODEL_H
#define QUERYMODEL_H

#include <QWidget>

namespace Ui {
class QueryModel;
}

class QueryModel : public QWidget
{
    Q_OBJECT

public:
    explicit QueryModel(QWidget *parent = nullptr);
    ~QueryModel();

private:
    Ui::QueryModel *ui;
};

#endif // QUERYMODEL_H
