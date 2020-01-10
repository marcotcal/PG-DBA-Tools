#ifndef QUERYMODEL_H
#define QUERYMODEL_H

#include "querymodeldata.h"
#include "connectionsdata.h"
#include <QWidget>
#include <QValidator>
#include <QTableWidgetItem>
#include <QFile>

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
    explicit QueryModel(ConnectionsData &connections, ProjectData &project, QWidget *parent = nullptr);
    ~QueryModel();
    void saveFile();
    void saveFileAs();
    bool openFile(QFile &file);
    bool canCloseOrReplace();
private slots:
    void on_bt_add_parameter_clicked();
    void on_bt_delete_parameter_clicked();
    void on_bt_test_parameter_clicked();
    void on_bt_add_order_clicked();
    void on_bt_delete_order_clicked();
    void on_bt_order_up_clicked();
    void on_bt_order_down_clicked();
    void on_bt_add_column_clicked();
    void on_bt_delete_column_clicked();
    void on_bt_column_up_clicked();
    void on_bt_column_down_clicked();
    void on_bt_test_query_clicked();    
    void on_sql_editor_textChanged();
    void on_parameter_table_itemChanged(QTableWidgetItem *item);
    void on_order_table_itemChanged(QTableWidgetItem *item);
    void on_column_table_itemChanged(QTableWidgetItem *item);

private:
    Ui::QueryModel *ui;
    ProjectData &project;
    QueryModelData data;
    void dataToEditors();
    void editorsToData();
    void initializeParameters();
    void initializeOrders();
    void initializeColumns();
    void initializeEditor();
    bool is_modified;
    QString file_name;
    QString default_path;
    ConnectionsData &connections;



};

#endif // QUERYMODEL_H
