#ifndef CONNECTIONLISTMODEL_H_
#define CONNECTIONLISTMODEL_H_

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>
#include <QList>
#include <QHash>

#include "connection.h"

namespace nsKonnectionMonitor
{

class ConnectionListProvider;

class ConnectionListModel
    : public QAbstractTableModel
{
    Q_OBJECT
public:
    ConnectionListModel(QObject* parent, ConnectionListProvider* connectionListProvider);
    virtual ~ConnectionListModel();

    //QAbstractTableModel impl
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const { Q_UNUSED(parent); return 5;}
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void refresh();

private:
    QList<Connection>* connections;
    ConnectionListProvider* connectionListProvider;
    QDateTime lastRefresh;
};

} // namespace nsKonnectionMonitor

#endif /*CONNECTIONLISTMODEL_H_*/
