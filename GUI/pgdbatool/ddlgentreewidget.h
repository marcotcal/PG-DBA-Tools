#ifndef DDLGENTREEVIEW_H
#define DDLGENTREEVIEW_H

#include <QTreeWidget>
#include <QMap>
#include <QString>
#include "pgdbaplugininterface.h"

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
    bool loadPlugins();
    QMap<QString, PGDBAPluginInterface *> interface_list;
};

#endif // DDLGENTREEVIEW_H
