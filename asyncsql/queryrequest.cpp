#include "queryrequest.h"
#include <functional>
#include <QDebug>

using namespace AsyncSql;

QueryRequest::QueryRequest(QObject *receiver, const QString &query, const QString &tableName, RequestType type) :
    receiver(receiver),
    query(query),
    tableName(tableName),
    sortColumn(-1),
    type(type)
{
    setRunBefore([](){});
    setCustomOperation([](){});
    setRunAfter([](){});
}

void QueryRequest::setReceiver(QObject *receiver) {
    this->receiver = receiver;
}

QObject* QueryRequest::getReceiver() const {
    return receiver;
}

void QueryRequest::setTableName(const QString &tableName) {
    this->tableName = tableName;
}

QString QueryRequest::getTableName() const {
    return tableName;
}

void QueryRequest::setQuery(const QString &query) {
    this->query = query;
}

QString QueryRequest::getQuery() const {
    return query;
}

void QueryRequest::setRequestType(RequestType type) {
    this->type = type;
}

QueryRequest::RequestType QueryRequest::getRequestType() const {
    return type;
}

void QueryRequest::setRecords(const QList<QSqlRecord> &records) {
    this->records = records;
}

QList<QSqlRecord> QueryRequest::getRecords() const {
    return records;
}

void QueryRequest::setRecord(const QSqlRecord &record) {
    this->records.clear();
    this->records << record;
}

void QueryRequest::setPrimaryIndex(const QSqlIndex &index) {
    primaryIndex = index;
}

QSqlIndex QueryRequest::getPrimaryIndex() const {
    return primaryIndex;
}

void QueryRequest::setSortColumn(int column) {
    sortColumn = column;
}

int QueryRequest::getSortColumn() const {
    return sortColumn;
}

void QueryRequest::setRunBefore(const std::function<void(void)> &process)
{
    runBefore = process;
}

std::function<void (void)> QueryRequest::getRunBefore() const
{
    return runBefore;
}

void QueryRequest::setCustomOperation(const std::function<void(void)> &process)
{
    customOperation = process;
}

std::function<void (void)> QueryRequest::getCustomOperation() const
{
    return customOperation;
}

void QueryRequest::setConnection(const DatabaseConnection &connection)
{
    this->connection = connection;
}

DatabaseConnection QueryRequest::getConnection() const
{
    return connection;
}

void QueryRequest::setRunAfter(const std::function<void(void)> &process)
{
    runAfter = process;
}

std::function<void (void)> QueryRequest::getRunAfter() const
{
    return runAfter;
}
