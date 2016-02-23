#include "queryworker.h"
#include <QDebug>
#include "databaseconnection.h"
#include "queryresult.h"
#include "queryrequest.h"
#include "querythread.h"
#include <QTime>

using namespace AsyncSql;

const QString sqliteConnectionName = "query_thread_sqlite";
const QString mysqlConnectionName = "query_thread_mysql";

QueryWorker::QueryWorker(QObject *parent) :
    QObject(parent)
{
    qDebug() << "QueryWorker thread started.";
}

void QueryWorker::execute(const QueryRequest &request) {
    QTime t;
    t.start();

    try {
        openConnection(request);

        error = QSqlError();
        result.setReceiver(request.getReceiver());

        QueryThread::setTransactionState(QueryThread::Unset);

        request.getRunBefore()();

        switch(request.getRequestType()) {
        case QueryRequest::Select:
            select(request);
            break;
        case QueryRequest::Insert:
            insert(request);
            break;
        case QueryRequest::Update:
            update(request);
            break;
        case QueryRequest::Delete:
            remove(request);
            break;
        case QueryRequest::BeginTransaction:
            startTransaction();
            break;
        case QueryRequest::CommitTransaction:
            commitTransaction();
            break;
        case QueryRequest::Command:
            executeCommand(request);
            break;
        case QueryRequest::CustomOperation:
            request.getCustomOperation()();
            break;
        }

        qDebug() << "QueryWorker: Results processed in " << t.elapsed() << " milliseconds.";

        setLastRecord(request);

        request.getRunAfter()();

        result.setRequestType(request.getRequestType());
        result.setStatus(true);
        emit resultsReady(result);
        result.clear();
    }
    catch (DatabaseException &e) {
        if(!error.isValid())
            error = e.getError();

        qDebug() << "Emitting failure results for " << request.getTableName();
        emit resultsReady(QueryResult(request.getReceiver(), error));
        result.clear();

        m_database.rollback();
        //unlockTables();
    }
    catch(std::exception &e) {
        qDebug() << "Unusual exception: " << e.what();
    }
    catch(...) {
        qDebug() << "Uncaught exception thrown....";
    }
}

void QueryWorker::openConnection(const QueryRequest &request)
{
    const QString hostName = request.getConnection().getHostName().trimmed().isEmpty() ?
                DatabaseConnection::getDefaultHostName() : request.getConnection().getHostName().trimmed();
    const int port = request.getConnection().getPort() == 0 ?
                DatabaseConnection::getDefaultPort() : request.getConnection().getPort();
    const QString userName = request.getConnection().getUserName().trimmed().isEmpty() ?
                DatabaseConnection::getDefaultUserName() : request.getConnection().getUserName().trimmed();
    const QString password = request.getConnection().getPassword().trimmed().isEmpty() ?
                DatabaseConnection::getDefaultPassword() : request.getConnection().getPassword().trimmed();
    const QString connectOptions = request.getConnection().getConnectOptions().trimmed().isEmpty() ?
                DatabaseConnection::getDefaultConnectOptions() : request.getConnection().getConnectOptions().trimmed();
    const QString driverName = request.getConnection().getDriverName().trimmed().isEmpty() ?
                DatabaseConnection::getDefaultDriverName() : request.getConnection().getDriverName().trimmed();
    const QString databaseName = request.getConnection().getDatabaseName().trimmed().isEmpty() ?
                DatabaseConnection::getDefaultDatabaseName() : request.getConnection().getDatabaseName().trimmed();

    if(m_database.hostName() != hostName
            || m_database.userName() != userName
            || m_database.password() != password
            || m_database.port() != port
            || m_database.databaseName() != databaseName
            || m_database.driverName() != driverName
            || m_database.connectOptions() != connectOptions)
        m_database.close();

    connectionName = driverName == "QSQLITE" ? sqliteConnectionName : mysqlConnectionName;

    if(!QSqlDatabase::contains(connectionName))
        m_database = QSqlDatabase::addDatabase(driverName, connectionName);
    else
        m_database = QSqlDatabase::database(connectionName);

    qDebug() << "Database object? " << m_database;

    m_database.setHostName(hostName);
    m_database.setUserName(userName);
    m_database.setPassword(password);
    m_database.setPort(port);
    m_database.setDatabaseName(databaseName);
    m_database.setConnectOptions(connectOptions);
    //m_database.setConnectOptions("MYSQL_OPT_RECONNECT = 1;");

    if(!m_database.open())
        throw DatabaseException(m_database.lastError());
}

void QueryWorker::select(const QueryRequest &request)
{
    QSqlQuery qry(m_database);

    if(request.getSortColumn() >= 0) {
        qry.prepare(QString("SELECT * FROM %1 LIMIT 0").arg(request.getTableName()));

        if(!qry.exec())
            throw DatabaseException(qry.lastError());

        QSqlRecord record(qry.record());

        qry.prepare(request.getQuery().arg(record.fieldName(request.getSortColumn())));
    }
    else
        qry.prepare(request.getQuery());

    if(!qry.exec()) {
        qDebug() << "Failed to execute query in query worker. The query was " << qry.lastQuery();
        qDebug() << "For table " << request.getTableName();
        qDebug() << qry.lastError();

        throw DatabaseException(qry.lastError());
    }

    result.setRecord(qry.record());
    result.setPrimaryIndex(m_database.primaryIndex(request.getTableName()));

    QList<QSqlRecord> records;
    while(qry.next()) {
        QSqlRecord record(qry.record());
        for(int i = 0; i < record.count(); ++i)
            record.setGenerated(i, false);

        records << record;
    }

    result.setRecords(records);
}

void QueryWorker::update(const QueryRequest &req)
{
    QueryRequest request(req);
    QSqlQuery qry(m_database);

    for(int i = 0; i < request.getRecords().count(); ++i) {
        QSqlRecord record(request.getRecords().at(i));
        QString query;

        for(int j = 0; j < record.count(); ++j) {
            if(!record.isGenerated(j))
                continue;

            query = QString("UPDATE %1 SET %2 = ? WHERE %3 = ?").arg(request.getTableName(),
                                                                 record.fieldName(j),
                                                                 request.getPrimaryIndex().fieldName(0));

            qry.prepare(query);
            qry.addBindValue(record.value(j));
            qry.addBindValue(record.value(request.getPrimaryIndex().fieldName(0)));

            if(!qry.exec()) {
                qDebug() << "Failed to update record in query worker.";
                qDebug() << qry.lastError();
                throw DatabaseException(qry.lastError());
            }
        }
    }

    qDebug() << "Update successful.";
}

void QueryWorker::insert(const QueryRequest &req)
{
    QueryRequest request(req);
    QSqlQuery qry(m_database);

    QList<QSqlRecord> records(request.getRecords());
    for(const QSqlRecord &record : records) {
        QString query;
        query = QString("INSERT INTO %1 (").arg(request.getTableName());

        for(int j = 0; j < record.count(); ++j) {
            query += record.fieldName(j);

            if(j != record.count() - 1)
                query += ", ";
            else
                query += ") ";
        }

        query += "VALUES (";

        // Prepare query
        for(int j = 0; j < record.count(); ++j) {
            query += "?";

            if(j != record.count() - 1)
                query += ", ";
            else
                query += ")";
        }

        qDebug() << "Prepared query for" << request.getTableName() << "in query worker? " << query;

        qry.prepare(query);

        for(int j = 0; j < record.count(); ++j)
            qry.addBindValue(record.value(j));

        if(!qry.exec()) {
            qDebug() << "Failed to insert record in query worker.";
            qDebug() << qry.lastError();
            throw DatabaseException(qry.lastError());
        }
    }

    qDebug() << "Insertion successful.";
    result.setLastInsertId(qry.lastInsertId());
}

void QueryWorker::remove(const QueryRequest &request)
{
    QSqlQuery qry(m_database);

    // Delete code
    for(int i = 0; i < request.getRecords().count(); ++i) {
        QSqlRecord record(request.getRecords().at(i));
        QString query;

        query = QString("DELETE FROM %1 WHERE %2 = ?").arg(request.getTableName(),
                                                           request.getPrimaryIndex().fieldName(0));

        qry.prepare(query);
        qry.addBindValue(record.value(request.getPrimaryIndex().fieldName(0)));

        if(!qry.exec()) {
            qDebug() << "Failed to delete record in query worker.";
            qDebug() << "The delete query in query worker for " << request.getTableName() << "? " << query;

            qDebug() << qry.lastError();
            throw DatabaseException(qry.lastError());
        }
    }

    qDebug() << "Delete successful.";
}

void QueryWorker::startTransaction()
{
    QSqlQuery q(m_database);
    q.prepare("SET autocommit = 0");

    if(!q.exec()) {
        qDebug() << "Failed to set autocommit to false.";
        throw DatabaseException(m_database.lastError());
    }

    if(!m_database.transaction()) {
        qDebug() << "Failed to begin transaction in QueryWorker::execute().";
        error = m_database.lastError();
        throw DatabaseException(m_database.lastError());
    }
}

void QueryWorker::commitTransaction()
{

    try {
        if(!m_database.commit()) {
            qDebug() << "Failed to commit transaction in QueryWorker::execute().";
            throw DatabaseException(m_database.lastError());
        }

        QSqlQuery q(m_database);
        q.prepare("SET autocommit = 1");

        if(!q.exec()) {
            qDebug() << "Failed to set autocommit to true.";
            throw DatabaseException(m_database.lastError());
        }

        //unlockTables();
    }
    catch(DatabaseException &) {
        //unlockTables();
        throw;
    }
}

void QueryWorker::executeCommand(const QueryRequest &request)
{
    try {
        QSqlQuery q(m_database);
        q.prepare(request.getQuery());

        if(!q.exec())
            throw DatabaseException(q.lastError());
    }
    catch(DatabaseException &) {
        throw;
    }
}

void QueryWorker::setLastRecord(const QueryRequest &request) {
    if(request.getTableName().trimmed().isEmpty())
        return;
    QSqlQuery q(m_database);
    q.prepare(QString("SELECT * FROM %1 ORDER BY %2 DESC LIMIT 1")
              .arg(request.getTableName(),
                   m_database.primaryIndex(request.getTableName()).fieldName(0)));
    if(!q.exec()) {
        qDebug() << "Failed to get last record for table " << request.getTableName();
        qDebug() << q.lastError();
        qDebug() << "Last query: " << q.lastQuery();
    }

    if(q.first())
        result.setLastRecord(q.record());
}

void QueryWorker::lockTables(const QueryRequest &request)
{
    try {
        QSqlQuery qry(m_database);
        qry.prepare(QString("LOCK TABLES %1 WRITE").arg(request.getTableName()));

        if(!qry.exec()) {
            throw DatabaseException(qry.lastError());
        }
    }
    catch(DatabaseException &) {
        throw;
    }
}

void QueryWorker::unlockTables()
{
    try {
        QSqlQuery qry(m_database);
        qry.prepare(QString("UNLOCK TABLES"));

        if(!qry.exec()) {
            throw DatabaseException(qry.lastError());
        }
    }
    catch(DatabaseException &) {
        throw;
    }
}

QueryWorker::~QueryWorker() {

}

QString QueryWorker::getSqliteConnectionName()
{
    return sqliteConnectionName;
}

QString QueryWorker::getMysqlConnectionName()
{
    return mysqlConnectionName;
}
