#include "ddlgenerationplugin.h"


DDLGenerationPlugin::DDLGenerationPlugin(QObject *parent) :
    QObject(parent)
{
}

QString DDLGenerationPlugin::version()
{
    return "VER 1.0.0";
}

QString DDLGenerationPlugin::plugin_name()
{
    return "DDLGenerator";
}

QString DDLGenerationPlugin::root_node_name()
{
    return QString();
}

QStringList DDLGenerationPlugin::itemNames()
{
    return QStringList();
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(pgdba_ddlgenplugin, DDLGenerationPlugin)
#endif // QT_VERSION < 0x050000