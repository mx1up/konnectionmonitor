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

        windowGeom = settings.value("general/windowGeometry").toByteArray();
        headerState = settings.value("general/header").toByteArray();
        autoRefresh = settings.value("general/autoRefresh", true).toBool();
        refreshInterval = settings.value("general/autoRefreshInterval", 1000).toInt();
        resolveHostnames = settings.value("general/resolveHostNames", true).toBool();
}

void Config::write()
{
        qDebug() << "Config::write()";

        QSettings settings;
        settings.setValue("general/autoRefresh", autoRefresh);
        settings.setValue("general/autoRefreshInterval", refreshInterval);
        settings.setValue("general/resolveHostNames", config->resolveHostnames);
}

} //namespace nsKonnectionMonitor
