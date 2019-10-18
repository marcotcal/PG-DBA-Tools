#include "dlgexecutemodel.h"
#include "ui_dlgexecutemodel.h"
#include <QFile>
#include <QXmlStreamReader>

DlgExecuteModel::DlgExecuteModel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgExecuteModel)
{
    ui->setupUi(this);
}

DlgExecuteModel::~DlgExecuteModel()
{
    delete ui;
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
