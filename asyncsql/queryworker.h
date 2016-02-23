#ifndef QUERYWORKER_H
#define QUERYWORKER_H

#include <QObject>
#include <QtSql>
#include "databaseexception.h"

#include "queryrequest.h"
#include "queryresult.h"

namespace AsyncSql {
class QueryWorker : public QObject
{
    Q_OBJECT
public:
    explicit QueryWorker(QObject *parent = 0);
    ~QueryWorker();

    static QString getSqliteConnectionName();
    static QString getMysqlConnectionName();
signals:
    void resultsReady(const QueryResult &);
public slots:
    void execute(const QueryRequest &);
private:
    QSqlDatabase m_database;
    QString connectionName;
    QList<QSqlRecord> recordsToInsert;
    QList<QSqlRecord> recordsToUpdate;
    QList<QSqlRecord> recordsToDelete;
    QueryResult result;
    QSqlError error;

    void openConnection(const QueryRequest &);

    void select(const QueryRequest &);
    void update(const QueryRequest &);
    void insert(const QueryRequest &);
    void remove(const QueryRequest &);
    void startTransaction();
    void commitTransaction();
    void executeCommand(const QueryRequest &);
    void setLastRecord(const QueryRequest &);

    void lockTables(const QueryRequest &);
    void unlockTables();
};
}

#endif // QUERYWORKER_H
