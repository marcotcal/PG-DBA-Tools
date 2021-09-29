#include "outputset.h"
#include "ui_outputset.h"

OutputSet::OutputSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputSet)
{
    ui->setupUi(this);
    type = OP_TEXT;
    ui->output_stack->setCurrentIndex(0);

#ifdef HML_USE_WEBENGINE
    html_output = new QWebEngineView(ui->pg_html);
#endif
#ifndef HML_USE_WEBENGINE
#ifdef HML_USE_WEBKIT
    html_output = new QWebView(ui->pg_html);
#endif
#ifndef HML_USE_WEBKIT
    html_output = new QTextBrowser(ui->pg_html);
    html_output->setReadOnly(true);
#endif
#endif

    html_output->setObjectName(QString::fromUtf8("html_output"));
    ui->html_layout->addWidget(html_output);
}

OutputSet::~OutputSet()
{
    delete ui;
}

QWidget *OutputSet::getOutput()
{
    switch(ui->output_stack->currentIndex()) {
    case 0:
        return ui->text_output;
        break;
    case 1:
        return ui->grid_output;
        break;
    case 2:
        return html_output;
        break;
    default:
        return nullptr;
    }
}

OutputSet::OutputType OutputSet::getType()
{
    return type;
}

void OutputSet::on_bt_txt_clicked()
{
    ui->output_stack->setCurrentIndex(0);
    type = OP_TEXT;
}

void OutputSet::on_bt_json_clicked()
{
    ui->output_stack->setCurrentIndex(0);
    type = OP_JSON;
}

void OutputSet::on_bt_xml_clicked()
{
    ui->output_stack->setCurrentIndex(0);
    type = OP_XML;
}

void OutputSet::on_bt_grid_clicked()
{
    ui->output_stack->setCurrentIndex(1);
    type = OP_GRID;
}

void OutputSet::on_bt_html_clicked()
{
    ui->output_stack->setCurrentIndex(2);
    type = OP_HTML;
}
