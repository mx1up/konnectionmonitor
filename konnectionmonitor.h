#ifndef KONNECTIONMONITOR_H
#define KONNECTIONMONITOR_H

#include <QTimer>
#include <QtGui/QMainWindow>
#include "ui_konnectionmonitor.h"

namespace nsKonnectionMonitor {

class ConnectionListModel;

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

    void setRefreshInterval(int interval);

protected:
    virtual void closeEvent(QCloseEvent* event);

private:
    void initGUI();
    void readConfig();
    void writeConfig();

    Ui::KonnectionMonitorClass ui;
    ConnectionListModel* connectionListModel;
    QTimer refreshTimer;
    int refreshInterval;
    bool resolveHostnames;
};

} // namespace 

#endif // KONNECTIONMONITOR_H
