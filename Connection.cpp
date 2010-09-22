#include "Connection.h"

namespace nsKonnectionMonitor
{

Connection::Connection(QString process, ConnectionType type, ConnectionState state, QHostAddress source, int sourcePort,
		QHostAddress dest, int destPort, QString sourceHostname, QString destHostname)
: process(process), type(type), state(state), source(source), sourcePort(sourcePort), dest(dest), destPort(destPort), sourceHostname(sourceHostname), destHostname(destHostname)
{
}

Connection::~Connection()
{
}

}
