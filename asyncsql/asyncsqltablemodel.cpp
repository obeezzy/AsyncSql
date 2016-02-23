#include "asyncsqltablemodel.h"
#include "queryrequest.h"
#include "queryresult.h"
#include "querythread.h"
#include <algorithm>

using namespace AsyncSql;

int AsyncSqlTableModel::transactionState = 0;

AsyncSqlTableModel::AsyncSqlTableModel(QObject *parent) :
    limit_(-1),
    sortColumn(-1),
    selectedSignalSuppressed(false),
    error(QSqlError()),
    submitCalled(false),
    selectType(All),
    currentRow_(-1),
    QAbstractTableModel(parent)
{
    connect(this, SIGNAL(execute(const QueryRequest &)),
            &QueryThread::getInstance(), SLOT(execute(const QueryRequest &)));
    connect(&QueryThread::getInstance(), SIGNAL(queryFinished(const QueryResult &)),
            this, SLOT(getResults(const QueryResult &)));
}

AsyncSqlTableModel::~AsyncSqlTableModel()
{
}

Qt::ItemFlags AsyncSqlTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags theFlags = QAbstractTableModel::flags(index);

    if (index.isValid())
        theFlags |= Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;

    return theFlags;
}

QVariant AsyncSqlTableModel::data(const QModelIndex &index, int role) const {
    if(records.isEmpty())
        return QVariant();
    if (!index.isValid() || index.row() < 0 || index.row() >= records.count()
            || index.column() < 0 || index.column() >= records.first().count())
        return QVariant();

    // For roles
    if(role >= Qt::UserRole + 1)
        return record(index.row()).value(fieldIndex(roleNames()[role]));
    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return records.at(index.row()).value(index.column());

    return QVariant();
}

QVariant AsyncSqlTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal) {
        if(records.isEmpty())
            return emptyRecord.fieldName(section);
        else
            return QVariant(records.first().fieldName(section));
    }
    else {
        if(removedRows.contains(section)) {
            //qDebug() << "The removed section is " << section;
            return "!";
        }
        if(insertedRows.contains(section)) {
            //qDebug() << "The inserted section is " << section;
            return "*";
        }
        return section + 1;
    }

    return QVariant();
}

int AsyncSqlTableModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : records.count();
}

int AsyncSqlTableModel::columnCount(const QModelIndex &parent) const {
    if(records.isEmpty())
        return emptyRecord.count();

    return parent.isValid() ? 0 : records.first().count();
}

bool AsyncSqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(records.isEmpty())
        return false;

    if (!index.isValid() || role != Qt::EditRole || index.row() < 0 || index.row() >= records.count()
            || index.column() < 0 || index.column() >= records.first().count())
        return false;

    QSqlRecord record(records.first());
    record.clearValues();
    record = records[index.row()];
    record.setValue(index.column(), value);
    records[index.row()] = record;

    if(!insertedRows.contains(index.row()) && !removedRows.contains(index.row()) &&
            !updatedRecordMap.contains(index.row()))
    {
        QSqlRecord record(emptyRecord);
        for(int i = 0; i < record.count(); ++i)
            record.setGenerated(i, false);

        // Set the primary key first
        record.setValue(primaryIndex.fieldName(0),
                        index.sibling(index.row(), record.indexOf(primaryIndex.fieldName(0))).data(Qt::EditRole));
        record.setValue(index.column(), value);
        // By default, QSqlRecord::isGenerated() returns false. I set the flag to true if the field
        // was updated.
        record.setGenerated(index.column(), true);
        updatedRecordMap.insert(index.row(), record);
    }
    else if(!insertedRows.contains(index.row()) && !removedRows.contains(index.row()) &&
            updatedRecordMap.contains(index.row()))
    {
        QSqlRecord record(updatedRecordMap.value(index.row()));
        // Set the primary key first
        record.setValue(primaryIndex.fieldName(0),
                        this->index(index.row(), record.indexOf(primaryIndex.fieldName(0))).data(Qt::EditRole));
        record.setValue(index.column(), value);
        // By default, QSqlRecord::isGenerated() returns false. I set the flag to true if the field
        // was updated.
        record.setGenerated(index.column(), true);
        updatedRecordMap.insert(index.row(), record);
    }

    emit dataChanged(index, index);
    return true;
}

bool AsyncSqlTableModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int) {
    return false;
}

bool AsyncSqlTableModel::insertRows(int row, int count, const QModelIndex&)
{
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        records.insert(row, emptyRecord);
        if(!insertedRows.contains(row)) {
            qDebug() << "Inserted row: " << row;
            insertedRows << row;
        }
    }
    endInsertRows();

    return true;
}

bool AsyncSqlTableModel::removeRows(int row, int count, const QModelIndex&)
{
    qDebug() << "What is row? " << row << " and count " << count;
    if((row < 0) || (row >= records.count()) || records.isEmpty())
        return false;

    if(originalRecords.contains(records.at(row))) {
        if(!removedRows.contains(row)) {
            removedRows << row;
        }

        qDebug() << "Removed rows? " << removedRows;
        insertedRows.removeAll(row);
        return true;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        records.removeAt(row);
    endRemoveRows();

    /*
    for(int i = 0; i < removedRows.count(); ++i) {
        removedRows[i] = removedRows.at(i) - count;
        insertedRows.removeAll(removedRows.at(i));
    }
    */

    insertedRows.removeAll(row);
    return true;
}

QModelIndexList AsyncSqlTableModel::match(const QModelIndex &start, int role,
                                     const QVariant &value, int hits,
                                     Qt::MatchFlags flags) const
{
    switch(value.type()) {
    case QVariant::Int:
    case QVariant::Double:
    {
        int count = 0;
        QModelIndexList foundIndexes;
        for(int i = 0; i < rowCount(); ++i) {
            QModelIndex cell = index(i, start.column());

            if(cell.data(Qt::EditRole).toDouble() == value.toDouble()) {
                foundIndexes << cell;
                count++;
            }

            if(count == hits)
                break;
        }

        return foundIndexes;
    }
        break;
    }

    return QAbstractTableModel::match(start, role, value, hits, flags);
}

void AsyncSqlTableModel::select() {
    QString query = QString("SELECT * FROM %1").arg(tableName_);

    if(!filter_.trimmed().isEmpty())
        query += " WHERE " + filter_.trimmed();

    if(sortColumn >= 0) {
        switch(order) {
        case Qt::AscendingOrder:
            query += QString(" ORDER BY %1");
            break;
        case Qt::DescendingOrder:
            query += QString(" ORDER BY %1 DESC");
            break;
        default:
            break;
        }
    }

    if(limit_ >= 0)
        query += " LIMIT " + QString::number(limit_);

    if(records.count())
        removeRows(0, this->records.count());
    records.clear();
    originalRecords.clear();
    insertedRows.clear();
    removedRows.clear();
    updatedRecordMap.clear();
    originalInsertedRows.clear();
    originalRemovedRows.clear();
    originalUpdatedRecordMap.clear();
    error = QSqlError();

    QueryRequest request(this, query, tableName_, QueryRequest::Select);
    request.setSortColumn(sortColumn);
    emit execute(request);
}

bool AsyncSqlTableModel::getResults(const QueryResult &result) {
    if(static_cast<AsyncSqlTableModel *>(result.getReceiver()) != this)
        return false;
    if(error.isValid())
        return false;
    if(result.getError().isValid()) {
        error = result.getError();

        if(result.getRequestType() == QueryRequest::Select)
            emit selected(false);
        else
            emit submitted(false);

        qDebug() << "Error discovered.";
        submitCalled = false;

        if(transactionState == 1)
            transactionState = -1;

        return false;
    }

    switch(result.getRequestType()) {
    case QueryRequest::Select:
    {
        beginResetModel();
        this->records = result.getRecords();
        originalRecords = result.getRecords();
        primaryIndex = result.getPrimaryIndex();
        lastRecord = result.getLastRecord();

        // Get empty record
        emptyRecord = result.getRecord();

        if(!selectedSignalSuppressed)
            emit selected(true);
        endResetModel();
    }
        break;
    case QueryRequest::Insert:
        insertedRows.clear();
        break;
    case QueryRequest::Update:
        updatedRecordMap.clear();
        break;
    case QueryRequest::Delete:
        removedRows.clear();
        break;
    case QueryRequest::BeginTransaction:
        break;
    case QueryRequest::CommitTransaction:
        break;
    }

    if(!insertedRows.count() && !updatedRecordMap.count() && !removedRows.count() && submitCalled) {
        submitCalled = false;
        emit submitted(true);
    }

    return true;
}

void AsyncSqlTableModel::setFilter(const QString &filter) {
    filter_ = filter;
}

QString AsyncSqlTableModel::filter() const {
    return filter_;
}

void AsyncSqlTableModel::setSort(int column, Qt::SortOrder order) {
    sortColumn = column;
    this->order = order;
}

void AsyncSqlTableModel::setTable(const QString &tableName) {
    tableName_ = tableName;
}

QString AsyncSqlTableModel::tableName() const {
    return tableName_;
}

bool AsyncSqlTableModel::isDirty() const {
    return !insertedRows.isEmpty() || !updatedRecordMap.isEmpty() || !removedRows.isEmpty() ;
}

bool AsyncSqlTableModel::isDirty(const QModelIndex &index) const {
    // Check if the insert rows or removed rows contains the index's row, or the
    // updated record map contains a field that is not generated i.e. that has been edited.
    return insertedRows.contains(index.row())
            || !updatedRecordMap.value(index.row()).field(index.column()).isGenerated()
            || removedRows.contains(index.row());
}

QList<QSqlRecord> AsyncSqlTableModel::getInsertedRecords() const {
    QList<QSqlRecord> records;

    for(auto &row : insertedRows)
        records << this->records.at(row);

    return records;
}

QMap<int, QSqlRecord> AsyncSqlTableModel::getUpdatedRecords() const {
    QMap<int, QSqlRecord> records(updatedRecordMap);

    return records;
}

QList<QSqlRecord> AsyncSqlTableModel::getRemovedRecords() const {
    QList<QSqlRecord> records;

    for(auto &row : removedRows)
        records << this->records.at(row);

    return records;
}

void AsyncSqlTableModel::setLastError(const QSqlError &e) {
    error = e;
}

QSqlError AsyncSqlTableModel::lastError() const {
    return error;
}

void AsyncSqlTableModel::setSelectType(SelectType s) {
    selectType = s;
}

AsyncSqlTableModel::SelectType AsyncSqlTableModel::getSelectType() const {
    return selectType;
}

void AsyncSqlTableModel::setCurrentRow(int row)
{
    if(currentRow_ == row)
        return;

    currentRow_ = row;
    emit currentRowChanged(row);
}

int AsyncSqlTableModel::currentRow() const
{
    return currentRow_;
}

QVariant AsyncSqlTableModel::field(const QString &columnName) const
{
    return record(currentRow_).value(columnName);
}

void AsyncSqlTableModel::beginTransaction() {
    transactionState = 1;
    QueryRequest request(this, "", tableName_, QueryRequest::BeginTransaction);
    emit execute(request);
}

void AsyncSqlTableModel::commitTransaction() {
    QueryRequest request(this, "", tableName_, QueryRequest::CommitTransaction);
    emit execute(request);
}

bool AsyncSqlTableModel::validateModel() {
    return true;
}

// WARNING: This function would only work if the primary key is not changed!!!
void AsyncSqlTableModel::submitAll() {
    if(insertedRows.isEmpty() && updatedRecordMap.isEmpty() && removedRows.isEmpty()) {
        emit submitted(true);
        return;
    }
    if(!validateModel())
        return;
    if(submitCalled)
        return;

    if(transactionState == -1) {
        insertedRows = originalInsertedRows;
        updatedRecordMap = originalUpdatedRecordMap;
        removedRows = originalRemovedRows;
    }
    else {
        originalUpdatedRecordMap = updatedRecordMap;
        originalInsertedRows = insertedRows;
        originalRemovedRows = removedRows;
    }

    qDebug() << "For " << tableName_ << ":";
    qDebug() << "What are the updated rows? " << updatedRecordMap.keys();
    qDebug() << "What are the inserted rows? " << insertedRows;
    qDebug() << "What are the removed rows? " << removedRows;
    qDebug() << "-----------------------------------------------------------";

    error = QSqlError();

    QueryRequest request(this);
    request.setTableName(tableName_);
    request.setPrimaryIndex(primaryIndex);
    QList<QSqlRecord> records;

    qDebug() << "Primary index? " << primaryIndex.fieldName(0) << ", auto value: "
             << primaryIndex.field(0).isAutoValue();

    // Update records
    if(!updatedRecordMap.isEmpty()) {
        request.setRequestType(QueryRequest::Update);
        records = updatedRecordMap.values();
        request.setRecords(records);
        emit execute(request);
    }


    // Insert records
    if(!insertedRows.isEmpty()) {
        request.setRequestType(QueryRequest::Insert);
        records.clear();
        for(int i = 0; i < insertedRows.count(); ++i) {
            qDebug() << "Inserted records: " << this->records.at(insertedRows.at(i));
            records << this->records.at(insertedRows.at(i));
        }
        request.setRecords(records);
        emit execute(request);
        records.clear();
    }


    // Delete records
    if(!removedRows.isEmpty()) {
        request.setRequestType(QueryRequest::Delete);
        for(int i = 0; i < removedRows.count(); ++i)
            records << this->records.at(removedRows.at(i));
        request.setRecords(records);
        emit execute(request);
        records.clear();
    }

    submitCalled = true;
}

void AsyncSqlTableModel::revertAll() {
    beginResetModel();
    records = originalRecords;
    insertedRows.clear();
    updatedRecordMap.clear();
    removedRows.clear();
    endResetModel();
}

QSqlRecord AsyncSqlTableModel::record() const {
    return emptyRecord;
}

QSqlRecord AsyncSqlTableModel::record(int row) const {
    if((row < 0) || (row >= records.count()))
        return emptyRecord;

    return records.at(row);
}

bool AsyncSqlTableModel::appendRecord(const QSqlRecord &record) {
    if(record.count() != this->record().count())
        return false;

    bool inserted = insertRow(rowCount());
    int row = rowCount() - 1;

    for(int i = 0; i < record.count(); ++i) {
        QModelIndex cell = index(row, i);
        setData(cell, record.value(i));
    }

    return inserted;
}

void AsyncSqlTableModel::setSelectedSignalSuppressed(bool s) {
    this->selectedSignalSuppressed = s;
}

bool AsyncSqlTableModel::isSelectedSignalSuppressed() const {
    return selectedSignalSuppressed;
}

void AsyncSqlTableModel::setRecords(const QList<QSqlRecord> &records) {
    this->records = records;
}

void AsyncSqlTableModel::setOriginalRecords(const QList<QSqlRecord> &records) {
    this->records = records;
}

QList<QSqlRecord> AsyncSqlTableModel::getRecords() const {
    return records;
}

QList<QSqlRecord> AsyncSqlTableModel::getOriginalRecords() const {
    return originalRecords;
}

QSqlRecord AsyncSqlTableModel::getLastRecord() const {
    return lastRecord;
}

void AsyncSqlTableModel::setLimit(int limit) {
    limit_ = limit;
}

int AsyncSqlTableModel::limit() const {
    return limit_;
}

int AsyncSqlTableModel::fieldIndex(const QString &fieldName) const {
    if(fieldName.trimmed().isEmpty())
        return -1;

    return record().indexOf(fieldName);
}

QList<int> AsyncSqlTableModel::getInsertedRows() const {
    return insertedRows;
}

bool AsyncSqlTableModel::setRecord(int row, const QSqlRecord &record) {
    if((row < 0) || (row >= rowCount()))
        return false;
    if(record.count() != emptyRecord.count())
        return false;

    for(int column = 0; column < record.count(); ++column) {
        QModelIndex cell = index(row, column);
        setData(cell, record.value(column));
    }

    return true;
}

void AsyncSqlTableModel::customInsert(const QVariantMap &values)
{
    if(values.isEmpty())
        return;
}
