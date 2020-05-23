#ifndef DDLGENTREEVIEW_H
#define DDLGENTREEVIEW_H


#include <QTreeWidget>

class DDLGenTreeWidget : public QTreeWidget
{    
    Q_OBJECT
public:
    DDLGenTreeWidget(QWidget *parent = Q_NULLPTR);

protected slots:

    void doItemDoubleClicked(QTreeWidgetItem *item, int column);

signals:

    void executeItem(QString resource_name);

};

#endif // DDLGENTREEVIEW_H
