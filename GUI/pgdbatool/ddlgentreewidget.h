#ifndef DDLGENTREEVIEW_H
#define DDLGENTREEVIEW_H

#include <QTreeWidget>
#include <QMap>
#include <QString>

class DDLGenTreeWidget : public QTreeWidget
{    
    Q_OBJECT
public:
    DDLGenTreeWidget(QWidget *parent = Q_NULLPTR);

protected slots:

    void doItemDoubleClicked(QTreeWidgetItem *item, int column);

signals:

    void executeItem(QString resource_name);

private:

};

#endif // DDLGENTREEVIEW_H
