#include "pluginelement.h"

PluginElement::PluginElement(PGDBAPluginInterface *interface, QJsonValue meta,
                             QObject *parent) :
    QObject(parent),
    interface(interface),
    meta(meta)
{

}

PGDBAPluginInterface *PluginElement::getInterface()
{
    return interface;
}

QJsonValue PluginElement::getMeta()
{
    return meta;
}
