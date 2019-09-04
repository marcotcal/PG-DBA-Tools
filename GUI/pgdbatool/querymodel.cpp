#include "querymodel.h"
#include "ui_querymodel.h"
#include <QFileDialog>
#include <QTextStream>
#include <QDomDocument>

QueryModel::QueryModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QueryModel)
{
    ui->setupUi(this);
}

QueryModel::~QueryModel()
{
    delete ui;
}

bool QueryModel::openFile()
{
    QString file_name;
    QDomDocument document;

    file_name = QFileDialog::getOpenFileName(this, "Open Model File", "./", "Model files (*.xml);;All files (*.*)");

    if(file_name != "") {

        QFile file(file_name);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return false;
        }
        else
        {
            if(!document.setContent(&file))
            {
                return false;
            }
            file.close();
        }
    }
    return false;
}

void QueryModel::saveFile()
{

}
