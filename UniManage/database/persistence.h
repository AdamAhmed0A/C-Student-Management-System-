#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>
#include <fstream>
#include <iostream>

class Persistence {
public:
    static void logChange(const QString& entity, const QString& action, int id, const QString& details) {
        std::ofstream logFile("D:/uni_changes_log.txt", std::ios::out | std::ios::app);
        if (logFile.is_open()) {
            std::string ts = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();
            logFile << "[" << ts << "] Entity: " << entity.toStdString() 
                    << " | Action: " << action.toStdString() 
                    << " | ID: " << id 
                    << " | Details: " << details.toStdString() << std::endl;
            logFile.close();
        }
    }

    static bool exportData(const QString& fileName, const QString& content) {
        std::string path = "D:/" + fileName.toStdString();
        std::ofstream outFile(path);
        if (outFile.is_open()) {
            outFile << content.toStdString();
            outFile.close();
            return true;
        }
        return false;
    }
};

#endif // PERSISTENCE_H
