#ifndef PLUGINELEMENT_H
#define PLUGINELEMENT_H

#include <QObject>
#include <pgdbaplugininterface.h>

class PluginElement : public QObject
{
    Q_OBJECT
public:
    explicit PluginElement(PGDBAPluginInterface *interface, QJsonValue meta,
                           QObject *parent = nullptr);
    PGDBAPluginInterface *getInterface();
    QJsonValue getMeta();
signals:

private:
    PGDBAPluginInterface *interface;
    QJsonValue meta;
public slots:

};

#endif // PLUGINELEMENT_H
