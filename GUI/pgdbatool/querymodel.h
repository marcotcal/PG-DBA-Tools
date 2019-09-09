#ifndef QUERYMODEL_H
#define QUERYMODEL_H

#include "querymodeldata.h"
#include "connectionsdata.h"
#include <QWidget>
#include <QValidator>

namespace Ui {
class QueryModel;
}

class UpercaseValidator: public QValidator {

    Q_OBJECT

public:
    UpercaseValidator(QObject* parent=nullptr): QValidator(parent) {}
    State validate(QString& input, int&) const override {
        input = input.toUpper();
        return QValidator::Acceptable;
    }
};

class QueryModel : public QWidget
{
    Q_OBJECT

public:
    explicit QueryModel(ConnectionsData &connections, QWidget *parent = nullptr);
    ~QueryModel();
    void saveFile();
    bool openFile();
private:
    Ui::QueryModel *ui;
    QueryModelData data;
    void initializeParameters();
    void initializeOrders();
    void initializeColumns();
    void initializeEditor();    
    bool is_modified;
    ConnectionsData &connections;
};

#endif // QUERYMODEL_H
