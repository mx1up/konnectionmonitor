#include "Connection.h"

namespace nsKonnectionMonitor
{

Connection::Connection(QString process, ConnectionType type, ConnectionState state, QHostAddress source, int sourcePort,
        QHostAddress dest, int destPort, QDateTime timestamp, QString sourceHostname, QString destHostname)
: process(process), type(type), state(state), source(source), sourcePort(sourcePort), dest(dest), destPort(destPort),
sourceHostname(sourceHostname), destHostname(destHostname), firstSeen(timestamp), lastSeen(timestamp)
{
}

Connection::~Connection()
{
}

bool Connection::operator==(const Connection &conn) const
{
    return source == conn.source && sourcePort == conn.sourcePort &&
            dest == conn.dest && destPort == conn.destPort;
}

} //namespace nsKonnectionMonitor
