#include "ddlgentreewidget.h"

DDLGenTreeWidget::DDLGenTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    QTreeWidgetItem* schemas = new QTreeWidgetItem();
    QTreeWidgetItem* triggers = new QTreeWidgetItem();
    QTreeWidgetItem* item;

    schemas->setText(0,"Schemas");
    triggers->setText(0,"Triggers");

    insertTopLevelItem(0, schemas);
    insertTopLevelItem(1,triggers);

    item = new QTreeWidgetItem(QStringList() << "Create Schema" << ":/ddl/ddl_generators/ddl_create_schemas.xml",1);
    schemas->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Drop Schema" << ":/ddl/ddl_generators/ddl_drop_schemas.xml",1);
    schemas->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Create Trigger" << ":/ddl/ddl_generators/ddl_create_triggers.xml",1);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Drop Trigger" << ":/ddl/ddl_generators/ddl_drop_triggers.xml",1);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Enable Trigger" << ":/ddl/ddl_generators/ddl_enable_triggers.xml",1);
    triggers->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Disable Trigger" << ":/ddl/ddl_generators/ddl_disable_triggers.xml",1);
    triggers->addChild(item);

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(doItemDoubleClicked(QTreeWidgetItem*,int)));

}

void DDLGenTreeWidget::doItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if (item->type() == 1)
        emit executeItem(item->text(1));
}
