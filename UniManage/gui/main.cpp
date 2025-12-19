#include <QApplication>
#include <QMessageBox>
#include "database/dbconnection.h"
#include "gui/loginwindow.h"
#include <QSqlDatabase>
#include <QProcessEnvironment>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application info
    app.setApplicationName("University SIS");
    app.setOrganizationName("University");
    
    // Debug: print available SQL drivers and DB env vars
    qDebug() << "Available SQL drivers:" << QSqlDatabase::drivers();
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    qDebug() << "DB_HOST=" << env.value("DB_HOST")
             << "DB_PORT=" << env.value("DB_PORT")
             << "DB_USER=" << env.value("DB_USER")
             << "DB_NAME=" << env.value("DB_NAME")
             << "(DB_PASSWORD hidden)";

    // Initialize database
    if (!DBConnection::instance().initialize()) {
        QString error = DBConnection::instance().getLastError();
        if (error.isEmpty()) error = "Failed to initialize database. Please check debug output.";
        
        QMessageBox::critical(nullptr, "Database Error", error);
        return 1;
    }
    
    // Show login window
    LoginWindow loginWindow;
    loginWindow.show();

    return app.exec();
}
