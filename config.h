#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QByteArray>

namespace nsKonnectionMonitor {

class Config
{
public:
    Config();
    virtual ~Config();

private:
    void read();
    void write();

public:
    bool debug;
    QString checkVersionURL;
    QString checkForUpdates;
    QByteArray windowGeom;
    QByteArray headerState;
    bool autoRefresh;
    int refreshInterval;
    bool resolveHostnames;
};

extern Config* config;

} // namespace nsKonnectionMonitor

#endif // CONFIG_H
