#ifndef DLGEXECUTEMODEL_H
#define DLGEXECUTEMODEL_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "querymodeldata.h"

namespace Ui {
class DlgExecuteModel;
}

class DlgExecuteModel : public QDialog
{
    Q_OBJECT

public:
    explicit DlgExecuteModel(QList<ModelItem *> &items, QWidget *parent = nullptr);
    ~DlgExecuteModel();
    QString selectedFileName();
    void setItems(QList<ModelItem *> &value) const { items = value; }
private slots:

    void on_model_tree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_model_tree_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::DlgExecuteModel *ui;
    void generateTree();
    QList<ModelItem *> &items;
    QTreeWidgetItem *selected;

};

#endif // DLGEXECUTEMODEL_H
