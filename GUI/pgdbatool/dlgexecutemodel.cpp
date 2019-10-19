#include "dlgexecutemodel.h"
#include "ui_dlgexecutemodel.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QDialogButtonBox>
#include <QPushButton>

DlgExecuteModel::DlgExecuteModel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgExecuteModel)
{
    ui->setupUi(this);
    readModels();
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

void DlgExecuteModel::readModels()
{
    QFile file("models.xml");
    QXmlStreamReader reader;
    QString code;
    QString description;
    QString file_name;
    QString model_path;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    reader.setDevice(&file);

    while(!reader.atEnd()) {

        while(reader.readNext()) {

            if(reader.isStartDocument())
                continue;

            if (reader.isEndDocument())
                break;

            if (reader.isEndElement()) {
                if (reader.name() == "model") {

                    items.append(new ModelItem(code, description, model_path, file_name, this));
                    code = "";
                    description = "";
                    file_name = "";
                    model_path = "";
                }
            }

            if (reader.isStartElement()) {

                if (reader.name() == "model") {
                    QXmlStreamAttributes attributes = reader.attributes();
                    if(attributes.hasAttribute("code"))
                         code = attributes.value("code").toString();
                    if(attributes.hasAttribute("description"))
                         description = attributes.value("description").toString();

                } else if (reader.name() == "model_path") {
                    model_path = reader.readElementText().trimmed();
                } else if (reader.name() == "file_name") {
                    file_name = reader.readElementText().trimmed();
                }

            }
        }
    }

   reader.clear();
   file.close();

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
    if (selected->text(1) != "") {
        accept();
    }
}
