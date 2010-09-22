#include "ConnectionListModel.h"

#include <QVariant>
#include <QtDebug>
#include "ConnectionListProvider.h"

namespace nsKonnectionMonitor
{

ConnectionListModel::ConnectionListModel(QObject* parent, ConnectionListProvider* connectionListProvider)
    : connectionListProvider(connectionListProvider)
{
    connections = new QList<Connection*>;
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

    if (role == Qt::DisplayRole) {
        Connection* connection = connections->at(index.row());
        switch (index.column()) {
        case 0:
            switch (connection->state) {
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
            case CLOSED:
                return "CLOSED";
            case CLSWAIT:
                return "CLOSEWAIT";
            case LASTACK:
                return "LASTACK";
            case LISTEN:
                return "LISTEN";
            case CLOSING:
                return "CLOSING";
            case UNKNOWN:
                return "UNKNOWN";
            default:
                return "OTHER_TODO";
            }
        case 1:
            switch (connection->type)
            {
            case TCP:
                return "TCP";
            case UDP:
                return "UDP";
            case RAW:
                return "RAW";
            }
        case 2:
            if (!connection->sourceHostname.isNull()) {
                return connection->sourceHostname + ":" + QString::number(connection->sourcePort);
            } else {
                return connection->source.toString() + ":"+ QString::number(connection->sourcePort);
            }
        case 3:

            if (!connection->destHostname.isNull()) {
                return connection->destHostname + ":" + QString::number(connection->destPort);
            } else {
                return connection->dest.toString() + ":"+ QString::number(connection->destPort);
            }
            // 				return connection->dest.toString() + ":"+ QString::number(connection->destPort);
        case 4:
            return connection->process;
        default:
            return QString("unknown column");
        }
    }
    else
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
    connections->clear();
    *connections << connectionListProvider->getConnectionList();
    updateView();
}

void ConnectionListModel::updateView()
{
    reset();
}

}
