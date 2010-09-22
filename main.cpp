#include "konnectionmonitor.h"

#include <QtGui>
#include <QApplication>

using namespace nsKonnectionMonitor;

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Mattiesworld");
    QCoreApplication::setOrganizationDomain("mattiesworld.gotdns.org");
    QCoreApplication::setApplicationName("KonnectionMonitor");

    QApplication app(argc, argv);
    KonnectionMonitor window;
    window.show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    return app.exec();
}
