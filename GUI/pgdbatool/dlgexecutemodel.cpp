#include "dlgexecutemodel.h"
#include "ui_dlgexecutemodel.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QDialogButtonBox>
#include <QPushButton>

DlgExecuteModel::DlgExecuteModel(QList<ModelItem *> &items, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgExecuteModel),
    items(items)
{
    ui->setupUi(this);
    generateTree();
}

DlgExecuteModel::~DlgExecuteModel()
{
    delete ui;
}

QString DlgExecuteModel::selectedFileName()
{
    if (selected)
        return selected->text(2);
    else {
        return "";
    }
}

void DlgExecuteModel::generateTree()
{
    QStringList menu;
    QTreeWidgetItem *root;
    QTreeWidgetItem *menu_element;
    QTreeWidgetItem *next_element = nullptr;
    QList<QTreeWidgetItem *> elements;
    QString code;
    QString description;
    QString file_name;

    root = new QTreeWidgetItem(ui->model_tree);
    root->setText(0, "Models");

    ui->model_tree->setColumnWidth(0,350);

    foreach (ModelItem *item, items) {

        menu = item->getMenuParts();
        code = item->getCode();
        file_name = item->getFileName();

        description = item->getDescription();
        menu_element = root;
        for(int i = 0; i < menu.count(); i++ ) {
            elements = ui->model_tree->findItems(menu.at(i), Qt::MatchExactly|Qt::MatchRecursive);
            foreach(QTreeWidgetItem *item, elements)
                if (item->parent() == menu_element) {
                    menu_element = item;
                    break;
                }
            if (elements.count() == 0) {
                next_element = new QTreeWidgetItem(menu_element, QStringList() << menu.at(i));
                menu_element->addChild(next_element);
                menu_element = next_element;
            }
            if (i == menu.count() - 1) {
                next_element = new QTreeWidgetItem(menu_element, QStringList() << code);
                menu_element->addChild(next_element);
                next_element->setText(1,description);
                next_element->setText(2,file_name);
            }
        }
    }

    ui->model_tree->expandItem(root);

 }

void DlgExecuteModel::on_model_tree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous)
    selected = current;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(selected->text(1) != "");
}

void DlgExecuteModel::on_model_tree_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    Q_UNUSED(item)
    if (selected->text(1) != "") {
        accept();
    }
}
