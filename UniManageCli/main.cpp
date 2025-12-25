#include <QCoreApplication>
#include <QDebug>
#include "cli.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("UniManageCLI");
    a.setOrganizationName("UniManage");

    Cli cli;
    cli.run();

    return a.exec();
}
