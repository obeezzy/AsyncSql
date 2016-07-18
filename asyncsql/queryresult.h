#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include <QObject>
#include <QtSql>
#include "queryrequest.h"

namespace AsyncSql {
class QueryResult
{
public:
    explicit QueryResult(QObject *receiver = 0, const QList<QSqlRecord> & = QList<QSqlRecord>());
    explicit QueryResult(QObject *receiver, QueryRequest::RequestType type, const QSqlError &);

    void clear();

    void setReceiver(QObject *);
    QObject *getReceiver() const;

    void setRequestType(QueryRequest::RequestType);
    QueryRequest::RequestType getRequestType() const;

    void setObjectName(const QString &);
    QString getObjectName() const;

    void setRecord(const QSqlRecord &);
    QSqlRecord getRecord() const;
    void setRecords(const QList<QSqlRecord> &records);
    QList<QSqlRecord> getRecords() const;

    void setPrimaryIndex(const QSqlIndex &);
    QSqlIndex getPrimaryIndex() const;

    void setError(const QSqlError &);
    QSqlError getError() const;

    void setLastRecord(const QSqlRecord &);
    QSqlRecord getLastRecord() const;

    void setLastInsertId(QVariant id);
    QVariant getLastInsertId() const;

    void setStatus(bool successful, const QString &msg = "");
    bool isSuccessful() const;
    QString getStatusMsg();
private:
    QueryRequest::RequestType requestType;
    QObject *receiver;
    QList<QSqlRecord> records;
    QSqlRecord record;
    QSqlIndex primaryIndex;
    QSqlError error;
    bool successful;
    QString statusMsg;
    QSqlRecord lastRecord;
    QString objectName;
    QVariant lastInsertId;
};
}

#endif // QUERYRESULT_H
