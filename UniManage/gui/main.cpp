#include <QApplication>
#include <QMessageBox>
#include "database/dbconnection.h"
#include "gui/loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application info
    app.setApplicationName("University SIS");
    app.setOrganizationName("University");
    
    // Initialize database
    if (!DBConnection::instance().initialize()) {
        QMessageBox::critical(nullptr, "Database Error", 
            "Failed to initialize database. Please check the logs.");
        return 1;
    }
    
    // Show login window
    LoginWindow loginWindow;
    loginWindow.show();

    return app.exec();
}
