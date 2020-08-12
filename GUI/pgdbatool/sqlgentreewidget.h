#ifndef SQLGENTREEWIDGET_H
#define SQLGENTREEWIDGET_H

#include <QTreeWidget>

class SQLGenTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    SQLGenTreeWidget(QWidget *parent = Q_NULLPTR);

protected slots:

    void doItemDoubleClicked(QTreeWidgetItem *item, int column);

signals:

    void executeItem(QString resource_name);

};

#endif // SQLGENTREEWIDGET_H
