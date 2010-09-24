//
// C++ Implementation: hostnameresolver
//
// Description: 
//
//
// Author: matthias sweertvaegher <gmatti3@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "hostnameresolver.h"

namespace nsKonnectionMonitor {

HostnameResolver::HostnameResolver(QHash<QString, QString>* hostnameCache, QObject *parent)
 : QObject(parent), hostnameCache(hostnameCache)
{
}


HostnameResolver::~HostnameResolver()
{
}

void HostnameResolver::lookedUp(QHostInfo hostInfo)
{
    if (hostInfo.error() != QHostInfo::NoError) {
        qDebug() << "Lookup failed:" << hostInfo.errorString();
        return;
    }

    foreach (QHostAddress address, hostInfo.addresses()) {
        qDebug() << "adding " << address.toString() << " -> " << hostInfo.hostName();
     	(*hostnameCache)[address.toString()] = hostInfo.hostName();
    }
}

} //namespace nsKonnectionMonitor
