#include "connection.h"

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

bool Connection::update(const Connection &conn)
{
    bool updated = false;
    if (sourceHostname != conn.sourceHostname) {
        sourceHostname = conn.sourceHostname;
        updated = true;
    }
    if (destHostname != conn.destHostname) {
        destHostname = conn.destHostname;
        updated = true;
    }
    if (state != conn.state) {
        state = conn.state;
        updated = true;
    }
    if (firstSeen.secsTo(conn.firstSeen) <= 3 ) updated = true;
    return updated;
}

bool Connection::isNew(QDateTime lastRefresh)
{
    return (firstSeen.secsTo(lastRefresh) < 3);
}

bool Connection::isStale(QDateTime lastRefresh)
{
    return (lastSeen.secsTo(lastRefresh) != 0);
}

} //namespace nsKonnectionMonitor
