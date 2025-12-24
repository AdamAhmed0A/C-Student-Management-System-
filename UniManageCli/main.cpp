#include <QCoreApplication>
#include <QDebug>
#include <cstdlib>
#include <iostream>
#include "cli.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("UniManageCLI");
    a.setOrganizationName("UniManage");

    // DBConnection is no longer needed for CLI as it uses file-based storage
    // if (!DBConnection::instance().initialize()) {
    //     qCritical() << "Failed to initialize database connection:" << DBConnection::instance().getLastError();
    //     return -1;
    // }

    Cli cli;
    cli.run();

    return a.exec();
}
