#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QString>

namespace AsyncSql {
class DatabaseConnection
{
public:
    enum Driver {Unknown, SQLite, MySQL};
    explicit DatabaseConnection();
    ~DatabaseConnection();

    void setHostName(const QString &hostName);
    QString getHostName() const;

    void setPort(int port);
    int getPort() const;

    void setUserName(const QString &userName);
    QString getUserName() const;

    void setPassword(const QString &password);
    QString getPassword() const;

    void setDriver(Driver);
    Driver getDriver() const;
    QString getDriverName() const;

    void setDatabaseName(const QString &databaseName);
    QString getDatabaseName() const;

    void setConnectOptions(const QString &options);
    QString getConnectOptions() const;


    // Default values
    static void setDefaultHostName(const QString &hostName);
    static QString getDefaultHostName();

    static void setDefaultPort(int port);
    static int getDefaultPort();

    static void setDefaultUserName(const QString &userName);
    static QString getDefaultUserName();

    static void setDefaultPassword(const QString &password);
    static QString getDefaultPassword();

    static void setDefaultDriver(Driver);
    static Driver getDefaultDriver();
    static QString getDefaultDriverName();

    static void setDefaultDatabaseName(const QString &databaseName);
    static QString getDefaultDatabaseName();

    static void setDefaultConnectOptions(const QString &options);
    static QString getDefaultConnectOptions();
private:
    QString userName;
    QString password;
    QString hostName;
    int port;
    QString driverName;
    QString databaseName;
    QString connectOptions;
    Driver driver;

    static QString defaultUserName;
    static QString defaultPassword;
    static QString defaultHostName;
    static int defaultPort;
    static QString defaultDriverName;
    static QString defaultDatabaseName;
    static QString defaultConnectOptions;
    static Driver defaultDriver;
};
}

#endif // DATABASECONNECTION_H
