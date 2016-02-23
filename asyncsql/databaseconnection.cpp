#include "databaseconnection.h"
#include <QDebug>

using namespace AsyncSql;

QString DatabaseConnection::defaultHostName = "";
int DatabaseConnection::defaultPort = 3306;

QString DatabaseConnection::defaultUserName = "";
QString DatabaseConnection::defaultPassword = "";
QString DatabaseConnection::defaultDriverName = "";
QString DatabaseConnection::defaultDatabaseName = "";
QString DatabaseConnection::defaultConnectOptions = "";
DatabaseConnection::Driver DatabaseConnection::defaultDriver = DatabaseConnection::Unknown;

DatabaseConnection::DatabaseConnection() :
    port(0)
{
    setDriver(Unknown);
}

DatabaseConnection::~DatabaseConnection()
{

}

void DatabaseConnection::setHostName(const QString &hostName)
{
    this->hostName = hostName;
}

QString DatabaseConnection::getHostName() const
{
    return hostName;
}

void DatabaseConnection::setPort(int port)
{
    this->port = port;
}

int DatabaseConnection::getPort() const
{
    return port;
}

void DatabaseConnection::setUserName(const QString &userName)
{
    this->userName = userName;
}

QString DatabaseConnection::getUserName() const
{
    return userName;
}

void DatabaseConnection::setPassword(const QString &password)
{
    this->password = password;
}

QString DatabaseConnection::getPassword() const
{
    return password;
}

void DatabaseConnection::setDriver(DatabaseConnection::Driver driver)
{
    this->driver = driver;

    switch(driver) {
    case SQLite:
        driverName = "QSQLITE";
        break;
    case MySQL:
        driverName = "QMYSQL";
        break;
    }
}

DatabaseConnection::Driver DatabaseConnection::getDriver() const
{
    return driver;
}

QString DatabaseConnection::getDriverName() const
{
    return driverName;
}

void DatabaseConnection::setDatabaseName(const QString &databaseName)
{
    this->databaseName = databaseName;
}

QString DatabaseConnection::getDatabaseName() const
{
    return databaseName;
}

void DatabaseConnection::setConnectOptions(const QString &options)
{
    this->connectOptions = options;
}

QString DatabaseConnection::getConnectOptions() const
{
    return connectOptions;
}

void DatabaseConnection::setDefaultHostName(const QString &hostName)
{
    DatabaseConnection::defaultHostName = hostName;
}

QString DatabaseConnection::getDefaultHostName()
{
    return defaultHostName;
}

void DatabaseConnection::setDefaultPort(int port)
{
    DatabaseConnection::defaultPort = port;
}

int DatabaseConnection::getDefaultPort()
{
    return defaultPort;
}

void DatabaseConnection::setDefaultUserName(const QString &userName)
{
    DatabaseConnection::defaultUserName = userName;
}

QString DatabaseConnection::getDefaultUserName()
{
    return defaultUserName;
}

void DatabaseConnection::setDefaultPassword(const QString &password)
{
    DatabaseConnection::defaultPassword = password;
}

QString DatabaseConnection::getDefaultPassword()
{
    return defaultPassword;
}

void DatabaseConnection::setDefaultDriver(DatabaseConnection::Driver driver)
{
    DatabaseConnection::defaultDriver = driver;

    switch(driver) {
    case SQLite:
        defaultDriverName = "QSQLITE";
        break;
    case MySQL:
        defaultDriverName = "QMYSQL";
        break;
    default:
        defaultDriverName = "";
        break;
    }
}

DatabaseConnection::Driver DatabaseConnection::getDefaultDriver()
{
    return defaultDriver;
}

QString DatabaseConnection::getDefaultDriverName()
{
    qDebug() << "Default driver name? " << defaultDriverName;
    return defaultDriverName;
}

void DatabaseConnection::setDefaultDatabaseName(const QString &databaseName)
{
    DatabaseConnection::defaultDatabaseName = databaseName;
}

QString DatabaseConnection::getDefaultDatabaseName()
{
    return defaultDatabaseName;
}

void DatabaseConnection::setDefaultConnectOptions(const QString &options)
{
    DatabaseConnection::defaultConnectOptions = options;
}

QString DatabaseConnection::getDefaultConnectOptions()
{
    return defaultConnectOptions;
}
