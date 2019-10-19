#ifndef DLGEXECUTEMODEL_H
#define DLGEXECUTEMODEL_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class DlgExecuteModel;
}

class ModelItem : public QObject
{

    Q_OBJECT

public:
    ModelItem(QString code, QString description, QString menu_path,
              QString file_name, QObject *parent=nullptr) :
        QObject(parent),
        code(code),
        description(description),
        menu_path(menu_path),
        file_name(file_name)
    {

    }
    QString getCode() { return code;}
    QString getDescription() { return description; }
    QString getMenuPath() { return menu_path; }
    QString getFileName() { return file_name; }
    QStringList getMenuParts() { return menu_path.split("/", QString::SkipEmptyParts); }
private:
    QString code;
    QString description;
    QString menu_path;
    QString file_name;
};

class DlgExecuteModel : public QDialog
{
    Q_OBJECT

public:
    explicit DlgExecuteModel(QWidget *parent = nullptr);
    ~DlgExecuteModel();
    QString selectedFileName();
private slots:

    void on_model_tree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_model_tree_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::DlgExecuteModel *ui;
    void readModels();
    void generateTree();
    QList<ModelItem *> items;
    QTreeWidgetItem *selected;
};

#endif // DLGEXECUTEMODEL_H
