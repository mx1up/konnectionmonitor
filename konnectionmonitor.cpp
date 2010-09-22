#include "konnectionmonitor.h"
#include "ConnectionListModel.h"

#include <QtDebug>
#include <QHeaderView>
#include <QSettings>
#include <QProcess>
#include <QApplication>
#include <QCoreApplication>

#include "ConnectionListProvider.h"

namespace nsKonnectionMonitor {

KonnectionMonitor::KonnectionMonitor(QWidget *parent)
    : QMainWindow(parent)
{
    //	connectionListModel = new ConnectionListModel(this, new MockConnectionListProvider());
    connectionListModel = new ConnectionListModel(this, new ProcNetConnectionListProvider());
    QSettings settings;
    refreshInterval = settings.value("general/autoRefreshInterval", 1000).toInt();
    initGUI();
}

KonnectionMonitor::~KonnectionMonitor()
{
    qDebug() << "~KonnectionMonitor ";
    //	writeConfig();
    delete connectionListModel;
}

void KonnectionMonitor::readConfig()
{
    qDebug() << "read config";
    QSettings settings;
}

void KonnectionMonitor::writeConfig()
{
    qDebug() << "write config";
    QSettings settings;
}

void KonnectionMonitor::initGUI()
{
    ui.setupUi(this);
    ui.autoRefreshSB->setValue(refreshInterval);

    ui.connectionTableView->horizontalHeader()->setStretchLastSection(true);
    ui.connectionTableView->verticalHeader()->hide();
    ui.connectionTableView->setModel(connectionListModel);
    connect(ui.refreshButton, SIGNAL(clicked()), this, SLOT(onRefreshButton_clicked()));
    connect(ui.refreshButton, SIGNAL(toggled(bool)), this, SLOT(onRefreshButton_toggled(bool)));


    connect(ui.autoRefreshCB, SIGNAL(toggled(bool)), this, SLOT(onAutoRefreshCB_toggled(bool)));
    connect(ui.autoRefreshSB, SIGNAL(valueChanged(int)), this, SLOT(setRefreshInterval(int)));
    connect(ui.rootModeButton, SIGNAL(clicked()), this, SLOT(onRootModeButton_clicked()));
    QProcess idProcess;
    idProcess.start("id -u");
    if (idProcess.waitForFinished(5000)) {
        if (QString(idProcess.readAllStandardOutput()).toInt() == 0) ui.rootModeButton->setEnabled(false);
    }

    connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(onRefreshButton_clicked()));

    QSettings settings;
    this->restoreGeometry(settings.value("general/windowGeometry").toByteArray());
    if (settings.value("general/columnWidth1", -1).toInt() == -1) {
        ui.connectionTableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        ui.connectionTableView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    } else {
        ui.connectionTableView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
        ui.connectionTableView->horizontalHeader()->resizeSection(0, settings.value("general/columnWidth1", -1).toInt());
        ui.connectionTableView->horizontalHeader()->resizeSection(1, settings.value("general/columnWidth2", -1).toInt());
        ui.connectionTableView->horizontalHeader()->resizeSection(2, settings.value("general/columnWidth3", -1).toInt());
        ui.connectionTableView->horizontalHeader()->resizeSection(3, settings.value("general/columnWidth4", -1).toInt());
    }

    ui.autoRefreshCB->setChecked(settings.value("general/autoRefresh", true).toBool());
}

void KonnectionMonitor::closeEvent(QCloseEvent* event)
{
    QSettings settings;
    settings.setValue("general/windowGeometry", saveGeometry());
    settings.setValue("general/columnWidth1", ui.connectionTableView->horizontalHeader()->sectionSize(0));
    settings.setValue("general/columnWidth2", ui.connectionTableView->horizontalHeader()->sectionSize(1));
    settings.setValue("general/columnWidth3", ui.connectionTableView->horizontalHeader()->sectionSize(2));
    settings.setValue("general/columnWidth4", ui.connectionTableView->horizontalHeader()->sectionSize(3));

    settings.setValue("general/autoRefreshInterval", refreshInterval);
    settings.setValue("general/autoRefresh", ui.autoRefreshCB->isChecked());
    // 	settings.setValue("general/resolveHostnames")

    QWidget::closeEvent(event);
}

void KonnectionMonitor::onRefreshButton_toggled(bool checked) {
    if (checked) {
        qDebug() << "start refresh timer with update interval = " << refreshInterval;
        ui.autoRefreshCB->setEnabled(false);
        ui.autoRefreshSB->setEnabled(false);
        refreshTimer.start(refreshInterval);
    } else {
        qDebug() << "stop refresh timer";
        ui.autoRefreshCB->setEnabled(true);
        ui.autoRefreshSB->setEnabled(true);
        refreshTimer.stop();
    }
}

void KonnectionMonitor::onRefreshButton_clicked() {
    connectionListModel->refresh();
}

void KonnectionMonitor::onAutoRefreshCB_toggled(bool checked)
{
    ui.refreshButton->setCheckable(checked);
}

void KonnectionMonitor::onRootModeButton_clicked()
{
    if (QProcess::startDetached(QString("kdesu -n -c %1").arg(QApplication::applicationFilePath()))) {
        close();
    }
}

void KonnectionMonitor::setRefreshInterval(int interval)
{
    refreshInterval = interval;
}

} // namespace
