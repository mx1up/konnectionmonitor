#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <QString>
#include <QHostAddress>
#include <QtCore/QDateTime>

namespace nsKonnectionMonitor
{

enum ConnectionType {
	TCP = 0,
    TCP6,
	UDP,
    UDP6,
	RAW
};

enum ConnectionState {
	ESTBLSH, SYNSENT, SYNRECV, FWAIT1, FWAIT2, TMEWAIT,
    CLOSE, CLSWAIT, LASTACK, LISTEN, CLOSING, MAX_STATES
};

class Connection
{
public:
	Connection(QString process, ConnectionType type, ConnectionState state, QHostAddress source,
               int sourcePort,	QHostAddress dest, int destPort, QDateTime timestamp = QDateTime::currentDateTime(),
               QString sourceHostname = QString(), QString destHostname = QString());
	virtual ~Connection();

    bool operator==(const Connection &conn) const;
    bool update(const Connection &conn);

    bool isNew(QDateTime lastRefresh);
    bool isStale(QDateTime lastRefresh);

	QString process;
	ConnectionType type;
	ConnectionState state;
	QHostAddress source; 
	int sourcePort;
	QHostAddress dest;
	int destPort;

	QString sourceHostname;
	QString destHostname;

    QDateTime firstSeen;
    QDateTime lastSeen;
};

} //namespace nsKonnectionMonitor

#endif /*CONNECTION_H_*/
