#include "konnectionmonitor.h"

#include "config.h"

#include <QtGui>
#include <QApplication>

using namespace nsKonnectionMonitor;

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Mattiesworld");
    QCoreApplication::setOrganizationDomain("mattiesworld.gotdns.org");
    QCoreApplication::setApplicationName("KonnectionMonitor");
    QCoreApplication::setApplicationVersion("v0.3.1");

    config = new Config();
    QApplication app(argc, argv);
    QStringList args = app.arguments();
    if (args.size() > 1 && args.at(1) == "debug") {
        config->debug = true;
    }

    KonnectionMonitor window;
    window.show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    return app.exec();
}
