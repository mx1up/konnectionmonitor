#include "config.h"

#include <QSettings>
#include <QtDebug>

namespace nsKonnectionMonitor {

Config* config;

Config::Config()
    : debug(false)
{
        read();
}

Config::~Config()
{
        write();
}

void Config::read()
{
        qDebug() << "Config::read()";
        QSettings settings;

        windowGeom = settings.value("windowGeometry").toByteArray();
        headerState = settings.value("header").toByteArray();
        autoRefresh = settings.value("autoRefresh", true).toBool();
        refreshInterval = settings.value("autoRefreshInterval", 1000).toInt();
        resolveHostnames = settings.value("resolveHostNames", true).toBool();
}

void Config::write()
{
        qDebug() << "Config::write()";

        QSettings settings;
        settings.setValue("autoRefresh", autoRefresh);
        settings.setValue("autoRefreshInterval", refreshInterval);
        settings.setValue("resolveHostNames", config->resolveHostnames);
}

} //namespace nsKonnectionMonitor
