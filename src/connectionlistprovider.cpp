#include "connectionlistprovider.h"

#include "config.h"

#include <QHostAddress>
#include <QList>
#include "connection.h"
#include <QStringList>
#include <QDir>
#include <QHash>
#include <QString>
#include <QHostAddress>
#include <QTextStream>
#include <QRegExp>

//#include <QtGlobal>

namespace nsKonnectionMonitor
{

ConnectionListProvider::ConnectionListProvider()
{
}

ConnectionListProvider::~ConnectionListProvider()
{

}

/////////////////////////////////////////////////////////////////

ProcNetConnectionListProvider::ProcNetConnectionListProvider()
    : ConnectionListProvider()
{
    hostnameResolver = new HostnameResolver(&hostnameCache);
    resolveHostnames = config->resolveHostnames;
}

ProcNetConnectionListProvider::~ProcNetConnectionListProvider()
{
    delete hostnameResolver;
}

void ProcNetConnectionListProvider::updateInodePidMap()
{
    inodeToPid.clear();
    // 	inodeToPid[15924] = 123;
    // 	qDebug() << "read /proc/<pid>/";
    QDir procDir("/proc");
    // 	QDir procDir("C:\\proc\\");
    qDebug() << "dir = "<< procDir.absolutePath() << procDir.dirName();
    if (!procDir.exists()) {
        qDebug() << "/proc/ does not exist!";
        return;
    }
    QStringList pidList = procDir.entryList(QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot);
    foreach (QString pid, pidList) {
        // 		qDebug() << "dir = " << pid;
        bool isNum;
        uint pidnum = pid.toUInt(&isNum);
        if (!isNum) {
            continue;
        }
        // 		qDebug() << "pid = " << pidnum;
        // 		QDir fdDir(QString("C:\\proc\\%1\\fd").arg(pid));
        QString fdDirStr = QString("/proc/%1/fd").arg(pid);
        QDir fdDir(fdDirStr);
        if (!fdDir.exists()) {
            qDebug() << fdDir.absolutePath() << "does not exist!";
            continue;
        }
        QFileInfoList fdList = fdDir.entryInfoList(QDir::System | QDir::Readable | QDir::Files);
        foreach (QFileInfo fd, fdList) {
            if (!fd.isSymLink()) {
                // 				qDebug() << fd.absolutePath() << "not a symlink";
                continue;
            }
            QString target = fd.symLinkTarget();
            // 			qDebug() << "symlink target" << target;
            QRegExp regexp("socket:\\[(\\d+)\\]");
            int pos = regexp.indexIn(target);
            if (pos > -1) {
                QString socketInode = regexp.cap(1);
                inodeToPid[socketInode.toUInt()] = pidnum;
            } else {
                // 				qDebug() << "not a socket!";
            }
        }
    }

}

QString ProcNetConnectionListProvider::getAppName(uint pid)
{
    if (pid == 0)
        return "<unknown>";


    QFile pidInfo(QString("/proc/%1/status").arg(pid));
    // 	QFile pidInfo(QString("C:/proc/%1/status").arg(pid));
    if (!pidInfo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "failed reading process info file";
        return "<no access>";
    }
    QTextStream stream(&pidInfo);
    QString line = stream.readLine();
    if (line.startsWith("Name:")) {
        return line.mid(6);
    }
    else {
        return "<unknown process info format>";
    }
}

QHostAddress ProcNetConnectionListProvider::convertProcNetAddress(QString address)
{
    switch(address.size()) {
    case 8: //ipv4
        address = address.mid(6, 2)+address.mid(4, 2) +address.mid(2, 2) +address.mid(0, 2);
        return QHostAddress((quint32)address.toUInt(NULL, 16));
        break;
    case 32: //ipv6
        return QHostAddress(address.toAscii().constData());
        break;
    default:
        qWarning() << "unknown address type for " << address;
        return QHostAddress();
    }

}

void ProcNetConnectionListProvider::parseProcNet(QString filename, ConnectionType connectionType, QList<Connection>* connectionList)
{
    qDebug() << "parsing" << filename;
    //	QFile file("/proc/net/tcp");
    QFile file(filename);
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text ) ) {
        qDebug() << "reading failed";
        return;
    }
    QTextStream stream( &file );
    QString line;
    bool isNum;
    do {
        line = stream.readLine();
        // 				qDebug() << line;
        if (line.isEmpty() || line.startsWith("  sl"))
            continue; // skip header and empty lines
        QStringList lineParts = line.split(' ', QString::SkipEmptyParts);


        //local host
        QStringList hostParts = lineParts.at(1).split(':');
        //		qDebug() << "local host  = " << hostParts.at(0).toUInt(&isNum, 16);
        //reverse numbers
        QHostAddress localHost = convertProcNetAddress(hostParts.at(0));
        //		qDebug() << "local host = " << localHost;
        //local port
        uint localPort = hostParts.at(1).toUInt(&isNum, 16);
        //		qDebug() << "local port = " << localPort;

        //remote host
        hostParts = lineParts.at(2).split(':');
        //reverse numbers
        QHostAddress remoteHost = convertProcNetAddress(hostParts.at(0));
        //		qDebug() << "remote host = " << remoteHost;
        //remote port
        uint remotePort = hostParts.at(1).toUInt(&isNum, 16);
        //		qDebug() << "remote port = " << remotePort;

        uint state = lineParts.at(3).toUInt(&isNum, 16);

        uint inodeNum = lineParts.at(9).toUInt(&isNum);
        //		qDebug() << "inode = " << inodeNum;

        // 		qDebug() << state << localHost << remoteHost << localPort << remotePort << inodeNum
        // 				<< getAppName(inodeToPid[inodeNum]);

        QString srcHostname;
        QString dstHostname;
        if (resolveHostnames) {
            if (hostnameCache.contains(localHost.toString())) {
                srcHostname = hostnameCache[localHost.toString()];
            } else {
                hostnameCache[localHost.toString()] = localHost.toString();
                qDebug() << "resolving" << localHost;
                QHostInfo::lookupHost(localHost.toString(), hostnameResolver, SLOT(lookedUp(QHostInfo)));
            }
            if (hostnameCache.contains(remoteHost.toString())) {
                dstHostname = hostnameCache[remoteHost.toString()];
            } else {
                hostnameCache[remoteHost.toString()] = remoteHost.toString();
                qDebug() << "resolving" << remoteHost;
                QHostInfo::lookupHost(remoteHost.toString(), hostnameResolver, SLOT(lookedUp(QHostInfo)));
            }
        }
        *connectionList
                << Connection(getAppName(inodeToPid[inodeNum]), connectionType, (ConnectionState)(state -1),
                                  localHost, localPort, remoteHost, remotePort, lastRefresh, srcHostname, dstHostname);
    } while (!line.isNull());
}

QList<Connection> ProcNetConnectionListProvider::getConnectionList(QDateTime timestamp)
{
    lastRefresh = timestamp;
    QList<Connection> connections;
    updateInodePidMap();
    // 	foreach (quint32 inode, inodeToPid.keys()) {
    // 		qDebug() << "inode -> pid " << inode << inodeToPid[inode];
    // 	}

    // 	parseProcNet(QFile("C:\\tests\\KonnectionMonitor\\test\\ProcNetParser\\tcpmock.txt"), &connections);
    parseProcNet("/proc/net/tcp", TCP, &connections);
    parseProcNet("/proc/net/tcp6", TCP6, &connections);
    parseProcNet("/proc/net/udp", UDP, &connections);
    parseProcNet("/proc/net/udp6", UDP6, &connections);
    return connections;
}

/////////////////////////////////////////////////////////////////

MockConnectionListProvider::MockConnectionListProvider()
{

}

MockConnectionListProvider::~MockConnectionListProvider()
{

}

QList<Connection> MockConnectionListProvider::getConnectionList(QDateTime timestamp)
{
    QList<Connection> connections;
    static int counter = 0;
    int nbConn = 5;
    for (int i=qMax(0, counter-nbConn); i<=counter; i++) {
        connections << Connection(QString("process %1").arg(counter), TCP, ESTBLSH, QHostAddress("127.0.0.0"), 1000+i, QHostAddress("127.0.0.0"), 1000+i, timestamp);
    }
    counter++;
    return connections;
}

} //namespace nsKonnectionMonitor
