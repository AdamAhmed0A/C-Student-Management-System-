#include <QApplication>
#include <QMessageBox>
#include "database/dbconnection.h"
#include "gui/loginwindow.h"
#include <QSqlDatabase>
#include <QProcessEnvironment>
#include <QDebug>
/**
 * Main entry point for the University Student Information System
 * Initializes the Qt application, sets up database connection, and displays the login window
 * @param argc - Number of command line arguments
 * @param argv - Array of command line argument strings
 * @return Application exit code (0 for success, 1 for database error)
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application info
    app.setApplicationName("University-SIS");
    app.setOrganizationName("University");
    

    // Show login window
    LoginWindow loginWindow;
    
    // Initialize database
    if (!DBConnection::instance().initialize()) {
        QString error = DBConnection::instance().getLastError();
        if (error.isEmpty()) error = "Failed to initialize database. Please check debug output.";
        
        QMessageBox::critical(nullptr, "Database Error", error);
        return 1;
    }
    
    loginWindow.show();

    return app.exec();
}
