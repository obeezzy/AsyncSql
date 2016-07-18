#include "queryresult.h"
#include "queryrequest.h"

using namespace AsyncSql;

QueryResult::QueryResult(QObject *receiver, const QList<QSqlRecord> &records) :
    receiver(receiver),
    requestType(QueryRequest::None),
    successful(false),
    records(records)
{
}

QueryResult::QueryResult(QObject *receiver, QueryRequest::RequestType type, const QSqlError &error) :
    receiver(receiver),
    requestType(type),
    successful(false),
    error(error)
{
}

void QueryResult::setRequestType(QueryRequest::RequestType type)
{
    requestType = type;
}

QueryRequest::RequestType QueryResult::getRequestType() const
{
    return requestType;
}

void QueryResult::setRecords(const QList<QSqlRecord> &records) {
    this->records = records;
}

QList<QSqlRecord> QueryResult::getRecords() const {
    return records;
}

void QueryResult::setPrimaryIndex(const QSqlIndex &index) {
    primaryIndex = index;
}

QSqlIndex QueryResult::getPrimaryIndex() const {
    return primaryIndex;
}

void QueryResult::setRecord(const QSqlRecord &record) {
    this->record = record;
    this->record.clearValues();
}

QSqlRecord QueryResult::getRecord() const {
    return record;
}

void QueryResult::setError(const QSqlError &error) {
    this->error = error;
}

QSqlError QueryResult::getError() const {
    return error;
}

void QueryResult::setReceiver(QObject *receiver) {
    this->receiver = receiver;
}

QObject *QueryResult::getReceiver() const {
    return receiver;
}

void QueryResult::setObjectName(const QString &name)
{
    objectName = name;
}

QString QueryResult::getObjectName() const
{
    return objectName;
}

void QueryResult::clear() {
    receiver = nullptr;
    requestType = QueryRequest::None;
    objectName = "";
    records.clear();
    record.clear();
    primaryIndex.clear();
    error = QSqlError();
    lastRecord.clear();
    successful = false;
    statusMsg = "";
}

void QueryResult::setStatus(bool successful, const QString &msg) {
    this->successful = successful;

    if(msg.trimmed().isEmpty())
        statusMsg = successful ? "SUCCEEDED" : "FAILED";
}

bool QueryResult::isSuccessful() const
{
    return successful;
}

QString QueryResult::getStatusMsg() {
    return statusMsg;
}

void QueryResult::setLastRecord(const QSqlRecord &r) {
    lastRecord = r;
}

QSqlRecord QueryResult::getLastRecord() const {
    return lastRecord;
}

void QueryResult::setLastInsertId(QVariant id)
{
    lastInsertId = id;
}

QVariant QueryResult::getLastInsertId() const
{
    return lastInsertId;
}
