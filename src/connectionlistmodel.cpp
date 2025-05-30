#include "connectionlistmodel.h"

#include "connection.h"
#include "connectionlistprovider.h"

#include <QColor>
#include <QVariant>
#include <QtDebug>

namespace nsKonnectionMonitor
{

ConnectionListModel::ConnectionListModel(QObject* parent, ConnectionListProvider* connectionListProvider)
    : connectionListProvider(connectionListProvider)
{
    connections = new QList<Connection>;
}

ConnectionListModel::~ConnectionListModel()
{
    qDebug() << "~ConnectionsModel";
}

int ConnectionListModel::rowCount(const QModelIndex &parent) const
{
    return connections->size();
}

QVariant ConnectionListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= connections->size())
        return QVariant();

    Connection connection = connections->at(index.row());
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            switch (connection.state) {
            case ESTBLSH:
                return "ESTABLISHED";
            case SYNSENT:
                return "SYNSENT";
            case SYNRECV:
                return "SYNRECV";
            case FWAIT1:
                return "FWAIT1";
            case FWAIT2:
                return "FWAIT2";
            case TMEWAIT:
                return "TIMEWAIT";
            case CLOSE:
                return "UNCONN";
            case CLSWAIT:
                return "CLOSEWAIT";
            case LASTACK:
                return "LASTACK";
            case LISTEN:
                return "LISTEN";
            case CLOSING:
                return "CLOSING";
            case MAX_STATES:
                return "MAX STATES";
            default:
                return "UNKNOWN";
            }
        case 1:
            switch (connection.type)
            {
            case TCP:
                return "TCP";
            case TCP6:
                return "TCP6";
            case UDP:
                return "UDP";
            case UDP6:
                return "UDP6";
            case RAW:
                return "RAW";
            default:
                return "UNKNOWN";
            }
        case 2:
            if (!connection.sourceHostname.isNull()) {
                return connection.sourceHostname + ":" + QString::number(connection.sourcePort);
            } else {
                return connection.source.toString() + ":"+ QString::number(connection.sourcePort);
            }
        case 3:

            if (!connection.destHostname.isNull()) {
                return connection.destHostname + ":" + QString::number(connection.destPort);
            } else {
                return connection.dest.toString() + ":"+ QString::number(connection.destPort);
            }
            // 				return connection.dest.toString() + ":"+ QString::number(connection.destPort);
        case 4:
            return connection.process;
        default:
            return QString("unknown column");
        }
    }

    if (role == Qt::BackgroundRole) {
        if (connection.isStale(lastRefresh)) return QColor(Qt::red);
        if (connection.isNew(lastRefresh)) return QColor(Qt::green);
        return QVariant();

    }

    return QVariant();
}

QVariant ConnectionListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section)
        {
        case 0:
            return QString("State");
        case 1:
            return QString("Type");
        case 2:
            return QString("Source");
        case 3:
            return QString("Destination");
        case 4:
            return QString("Process name");
        default:
            return QString("Undefined column");
        }
    }
    else {
        return QVariant();
        //return QString("%1").arg(section+1);
    }

}

void ConnectionListModel::refresh()
{
    lastRefresh = QDateTime::currentDateTime();
    QList<Connection> newConnections = connectionListProvider->getConnectionList(lastRefresh);
    foreach (Connection conn, newConnections) {
        if (connections->contains(conn)) {
            int existingConnIndex = connections->indexOf(conn);
            Connection& existingConn = (*connections)[existingConnIndex];
            existingConn.lastSeen = conn.firstSeen;
            if (existingConn.update(conn)) {
                dataChanged(index(existingConnIndex, 0),index(existingConnIndex, 4));
            }
        } else {
            beginInsertRows(QModelIndex(), connections->size(), connections->size());
            connections->append(conn);
            endInsertRows();
        }
    }
    QDateTime now = QDateTime::currentDateTime();
    QList<Connection>::iterator it;
    for (it = connections->begin(); it != connections->end();) {
        Connection conn = *it;
        if (conn.lastSeen.secsTo(now) > 0) {
            int rowIndex = connections->indexOf(conn);
            if (conn.lastSeen.secsTo(now) > 5) {
                beginRemoveRows(QModelIndex(), rowIndex, rowIndex);
                it = connections->erase(it);
                endRemoveRows();
            } else {
                dataChanged(index(rowIndex, 0),index(rowIndex, 4));
                ++it;
            }
        } else {
            ++it;
        }
    }
}

} //namespace nsKonnectionMonitor
