#ifndef CONNECTIONLISTPROVIDER_H_
#define CONNECTIONLISTPROVIDER_H_

#include <QList>
#include "Connection.h"
#include <QHash>
#include <QHostInfo>

#include "hostnameresolver.h"

namespace nsKonnectionMonitor
{

class ConnectionListProvider
{
public:
    ConnectionListProvider();
    virtual ~ConnectionListProvider();

    virtual QList<Connection> getConnectionList(QDateTime timestamp) = 0;
};
/*
class HostnameResolver : public QObject
{
Q_OBJECT
public:
HostnameResolver(QHash<QString, QString>* hostnameCache);
virtual ~HostnameResolver();

public slots:
void lookedUp(QHostInfo hostInfo);

private:
QHash<QString, QString>* hostnameCache;
};
*/
class ProcNetConnectionListProvider : public ConnectionListProvider
{
public:
    ProcNetConnectionListProvider();
    virtual ~ProcNetConnectionListProvider();

    virtual QList<Connection> getConnectionList(QDateTime timestamp);

private:
    void updateInodePidMap();
    QString getAppName(uint pid);
    quint32 convertProcNetAddressToInt(QString address);
    void parseProcNet(QString filename, QList<Connection>* connectionList);

    QHash<quint32,quint32> inodeToPid;
    bool resolveHostnames;
    QHash<QString, QString> hostnameCache;
    HostnameResolver* hostnameResolver;
    QDateTime lastRefresh;
};

class MockConnectionListProvider : public ConnectionListProvider
{
public:
    MockConnectionListProvider();
    virtual ~MockConnectionListProvider();

    virtual QList<Connection> getConnectionList(QDateTime timestamp);
};

} //namespace nsKonnectionMonitor

#endif /*CONNECTIONLISTPROVIDER_H_*/
