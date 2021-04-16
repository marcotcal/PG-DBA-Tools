#ifndef PLUGINTREEWIDGET_H
#define PLUGINTREEWIDGET_H

#include <QObject>
#include <QTreeWidget>
#include <QListWidget>
#include <pluginelement.h>

class PluginTreeWidget : public QTreeWidget
{

    Q_OBJECT

public:
    PluginTreeWidget(QWidget *parent=nullptr);

    QListWidget *getlistWidget() const;
    void setListWidget(QListWidget *value);

    PluginElement *getElement() const;
    void setElement(PluginElement *value);

private:
    QListWidget *list_widget;
    PluginElement *element;

};

#endif // PLUGINTREEWIDGET_H
