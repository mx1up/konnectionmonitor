#ifndef KONNECTIONMONITOR_H
#define KONNECTIONMONITOR_H

#include <QTimer>
#include <QtGui/QMainWindow>
#include "ui_konnectionmonitor.h"

class QSortFilterProxyModel;

namespace nsKonnectionMonitor {

class ConnectionListModel;
class AboutWidget;

class KonnectionMonitor : public QMainWindow
{
    Q_OBJECT

public:
    KonnectionMonitor(QWidget *parent = 0);
    ~KonnectionMonitor();
    
public slots:
    void onRefreshButton_clicked();
    void onRefreshButton_toggled(bool checked);
    void onAutoRefreshCB_toggled(bool checked);
    void onRootModeButton_clicked();
    void onActionAbout_triggered();
    void setRefreshInterval(int interval);

protected:
    virtual void closeEvent(QCloseEvent* event);

private:
    bool checkRootMode();
    void initGUI();

    Ui::KonnectionMonitorClass ui;
    AboutWidget *aboutDialog;

    ConnectionListModel* connectionListModel;
    QSortFilterProxyModel* filterModel;
    QTimer refreshTimer;
    bool rootMode;
};

} // namespace nsKonnectionMonitor

#endif // KONNECTIONMONITOR_H
