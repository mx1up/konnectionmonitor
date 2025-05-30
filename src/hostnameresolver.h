//
// C++ Interface: hostnameresolver
//
// Description: 
//
//
// Author: matthias sweertvaegher, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NSKONNECTIONMONITORHOSTNAMERESOLVER_H
#define NSKONNECTIONMONITORHOSTNAMERESOLVER_H

#include <QObject>
#include <QHostInfo>
#include <QString>
#include <QHash>

namespace nsKonnectionMonitor {

/**
@author matthias sweertvaegher
*/
class HostnameResolver : public QObject
{
    Q_OBJECT
public:
    HostnameResolver(QHash<QString, QString>* hostnameCache, QObject *parent = 0);
    virtual ~HostnameResolver();

public slots:
    void lookedUp(QHostInfo hostInfo);

private:
    QHash<QString, QString>* hostnameCache;
};

} //namespace nsKonnectionMonitor

#endif
