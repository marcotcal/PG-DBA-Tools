#include "sqlgentreewidget.h"

SQLGenTreeWidget::SQLGenTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
    QTreeWidgetItem* inserts = new QTreeWidgetItem();
    QTreeWidgetItem* updates = new QTreeWidgetItem();
    QTreeWidgetItem* selects = new QTreeWidgetItem();
    QTreeWidgetItem* item;

    inserts->setText(0,"Inserts");
    updates->setText(0,"Updates");
    selects->setText(0,"Selects");

    insertTopLevelItem(0, inserts);
    insertTopLevelItem(1, updates);
    insertTopLevelItem(2, selects);

    item = new QTreeWidgetItem(QStringList() << "Insert in Table" << ":/ddl/ddl_generators/ddl_create_schemas.xml",1);
    inserts->addChild(item);

    item = new QTreeWidgetItem(QStringList() << "Insert in Table (Only Mandatory)" << ":/ddl/ddl_generators/ddl_create_schemas.xml",1);
    inserts->addChild(item);

    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(doItemDoubleClicked(QTreeWidgetItem*,int)));

}

void SQLGenTreeWidget::doItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if (item->type() == 1)
        emit executeItem(item->text(1));
}
