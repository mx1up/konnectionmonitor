#include "konnectionmonitor.h"
#include "connectionlistmodel.h"
#include "config.h"
#include "connectionlistprovider.h"
#include "aboutwidget.h"

#include <QtDebug>
#include <QtGui/QHeaderView>
#include <QtCore/QSettings>
#include <QtCore/QProcess>
#include <QtGui/QApplication>
#include <QtCore/QCoreApplication>
#include <QtGui/QSortFilterProxyModel>


namespace nsKonnectionMonitor {

KonnectionMonitor::KonnectionMonitor(QWidget *parent)
    : QMainWindow(parent), aboutDialog(0)
{
    rootMode = checkRootMode();
    if (config->debug) {
        connectionListModel = new ConnectionListModel(this, new MockConnectionListProvider());
    } else {
        connectionListModel = new ConnectionListModel(this, new ProcNetConnectionListProvider());
    }
    initGUI();
}

KonnectionMonitor::~KonnectionMonitor()
{
    qDebug() << "~KonnectionMonitor ";
    delete connectionListModel;
    delete config;
}

bool KonnectionMonitor::checkRootMode()
{
    QProcess idProcess;
    idProcess.start("id -u");
    if (idProcess.waitForFinished(5000)) {
        return QString(idProcess.readAllStandardOutput()).toInt() == 0;
    }
    return false;
}

void KonnectionMonitor::initGUI()
{
    ui.setupUi(this);
    ui.autoRefreshSB->setValue(config->refreshInterval);

    ui.connectionTableView->horizontalHeader()->setStretchLastSection(true);
    ui.connectionTableView->verticalHeader()->setDefaultSectionSize(ui.connectionTableView->verticalHeader()->minimumSectionSize());
    ui.connectionTableView->verticalHeader()->hide();
    filterModel = new QSortFilterProxyModel(this);
    filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filterModel->setFilterKeyColumn(-1);
    filterModel->setDynamicSortFilter(true);
    filterModel->setSourceModel(connectionListModel);
    ui.connectionTableView->setModel(filterModel);
    connect(ui.refreshButton, SIGNAL(clicked()), this, SLOT(onRefreshButton_clicked()));
    connect(ui.refreshButton, SIGNAL(toggled(bool)), this, SLOT(onRefreshButton_toggled(bool)));


    connect(ui.autoRefreshCB, SIGNAL(toggled(bool)), this, SLOT(onAutoRefreshCB_toggled(bool)));
    connect(ui.autoRefreshSB, SIGNAL(valueChanged(int)), this, SLOT(setRefreshInterval(int)));
    connect(ui.rootModeButton, SIGNAL(clicked()), this, SLOT(onRootModeButton_clicked()));
    ui.rootModeButton->setEnabled(!rootMode);
    ui.rootModeButton->setToolTip(rootMode ? "Root mode activated" : "Activate root mode");

    connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(onRefreshButton_clicked()));
    connect(ui.filterLE, SIGNAL(textChanged(QString)), filterModel, SLOT(setFilterRegExp(QString)));

    this->restoreGeometry(config->windowGeom);
    if (!ui.connectionTableView->horizontalHeader()->restoreState(config->headerState)) {
        ui.connectionTableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        ui.connectionTableView->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
    }

    ui.autoRefreshCB->setChecked(config->autoRefresh);
    ui.refreshButton->click();

    connect(ui.aboutButton, SIGNAL(clicked()), this, SLOT(onActionAbout_triggered()));
}

void KonnectionMonitor::closeEvent(QCloseEvent* event)
{
    QSettings settings;
    settings.setValue("general/windowGeometry", saveGeometry());
    settings.setValue("general/header", ui.connectionTableView->horizontalHeader()->saveState());

    QWidget::closeEvent(event);
}

void KonnectionMonitor::onRefreshButton_toggled(bool checked) {
    if (checked) {
        qDebug() << "start refresh timer with update interval = " << config->refreshInterval;
        ui.autoRefreshCB->setEnabled(false);
        ui.autoRefreshSB->setEnabled(false);
        refreshTimer.start(config->refreshInterval);
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
    ui.refreshButton->setCheckable(config->autoRefresh = checked);
}

void KonnectionMonitor::onRootModeButton_clicked()
{
    if (QProcess::startDetached(QString("xdg-su -c %1").arg(QApplication::applicationFilePath()))) {
        close();
    }
}

void KonnectionMonitor::setRefreshInterval(int interval)
{
    config->refreshInterval = interval;
}

void KonnectionMonitor::onActionAbout_triggered() {
    if (aboutDialog == 0) {
        aboutDialog = new AboutWidget(this);
        aboutDialog->setVersion(qApp->applicationVersion());
        aboutDialog->adjustSize();
    }
    aboutDialog->exec();
}

} // namespace nsKonnectionMonitor
