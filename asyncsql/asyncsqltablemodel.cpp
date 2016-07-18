#include "asyncsqltablemodel.h"
#include "queryrequest.h"
#include "queryresult.h"
#include "querythread.h"
#include <algorithm>

using namespace AsyncSql;

AsyncSqlTableModel::AsyncSqlTableModel(QObject *parent) :
    limit_(-1),
    sortColumn_(-1),
    order_(Qt::AscendingOrder),
    selectedSignalSuppressed_(false),
    error_(QSqlError()),
    submitCalled_(false),
    currentRow_(-1),
    foreignKeyFlag_(true),
    busy_(false),
    QAbstractTableModel(parent)
{
    connect(this, SIGNAL(execute(const QueryRequest &)),
            &QueryThread::instance(), SLOT(execute(const QueryRequest &)));
    connect(&QueryThread::instance(), SIGNAL(queryFinished(const QueryResult &)),
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
    if(records_.isEmpty())
        return QVariant();
    if (!index.isValid() || index.row() < 0 || index.row() >= records_.count()
            || index.column() < 0 || index.column() >= records_.first().count())
        return QVariant();

    // For roles
    if(role >= Qt::UserRole + 1)
        return record(index.row()).value(fieldIndex(roleNames()[role]));
    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return records_.at(index.row()).value(index.column());

    return QVariant();
}

QVariant AsyncSqlTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal) {
        if(records_.isEmpty())
            return emptyRecord_.fieldName(section);
        else
            return QVariant(records_.first().fieldName(section));
    }
    else {
        if(removedRows_.contains(section)) {
            //qDebug() << "The removed section is " << section;
            return "!";
        }
        if(insertedRows_.contains(section)) {
            //qDebug() << "The inserted section is " << section;
            return "*";
        }
        return section + 1;
    }

    return QVariant();
}

int AsyncSqlTableModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : records_.count();
}

int AsyncSqlTableModel::columnCount(const QModelIndex &parent) const {
    if(records_.isEmpty())
        return emptyRecord_.count();

    return parent.isValid() ? 0 : records_.first().count();
}

bool AsyncSqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if(records_.isEmpty())
        return false;

    if (!index.isValid() || role != Qt::EditRole || index.row() < 0 || index.row() >= records_.count()
            || index.column() < 0 || index.column() >= records_.first().count())
        return false;

    QSqlRecord record(records_.first());
    record.clearValues();
    record = records_[index.row()];
    record.setValue(index.column(), value);
    records_[index.row()] = record;

    if(!insertedRows_.contains(index.row()) && !removedRows_.contains(index.row()) &&
            !updatedRecordMap_.contains(index.row()))
    {
        QSqlRecord record(emptyRecord_);
        for(int i = 0; i < record.count(); ++i)
            record.setGenerated(i, false);

        // Set the primary key first
        record.setValue(primaryIndex_.fieldName(0),
                        index.sibling(index.row(), record.indexOf(primaryIndex_.fieldName(0))).data(Qt::EditRole));
        record.setValue(index.column(), value);
        // By default, QSqlRecord::isGenerated() returns false. I set the flag to true if the field
        // was updated.
        record.setGenerated(index.column(), true);
        updatedRecordMap_.insert(index.row(), record);
    }
    else if(!insertedRows_.contains(index.row()) && !removedRows_.contains(index.row()) &&
            updatedRecordMap_.contains(index.row()))
    {
        QSqlRecord record(updatedRecordMap_.value(index.row()));
        // Set the primary key first
        record.setValue(primaryIndex_.fieldName(0),
                        this->index(index.row(), record.indexOf(primaryIndex_.fieldName(0))).data(Qt::EditRole));
        record.setValue(index.column(), value);
        // By default, QSqlRecord::isGenerated() returns false. I set the flag to true if the field
        // was updated.
        record.setGenerated(index.column(), true);
        updatedRecordMap_.insert(index.row(), record);
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
        records_.insert(row, emptyRecord_);
        if(!insertedRows_.contains(row)) {
            qDebug() << "Inserted row: " << row;
            insertedRows_ << row;
        }
    }
    endInsertRows();

    return true;
}

bool AsyncSqlTableModel::removeRows(int row, int count, const QModelIndex&)
{
    qDebug() << "What is row? " << row << " and count " << count;
    if((row < 0) || (row >= records_.count()) || records_.isEmpty())
        return false;

    if(originalRecords_.contains(records_.at(row))) {
        if(!removedRows_.contains(row)) {
            removedRows_ << row;
        }

        qDebug() << "Removed rows? " << removedRows_;
        insertedRows_.removeAll(row);
        return true;
    }

    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i = 0; i < count; ++i)
        records_.removeAt(row);
    endRemoveRows();

    /*
    for(int i = 0; i < removedRows_.count(); ++i) {
        removedRows_[i] = removedRows_.at(i) - count;
        insertedRows_.removeAll(removedRows_.at(i));
    }
    */

    insertedRows_.removeAll(row);
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

    if(sortColumn_ >= 0) {
        switch(order_) {
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

    if(records_.count())
        removeRows(0, records_.count());
    records_.clear();
    originalRecords_.clear();
    insertedRows_.clear();
    removedRows_.clear();
    updatedRecordMap_.clear();
    originalInsertedRows_.clear();
    originalRemovedRows_.clear();
    originalUpdatedRecordMap_.clear();
    error_ = QSqlError();

    setBusy(true);

    QueryRequest request(this, query, tableName_, QueryRequest::Select);
    request.setSortColumn(sortColumn_);
    emit execute(request);
}

bool AsyncSqlTableModel::getResults(const QueryResult &result) {
    if(static_cast<AsyncSqlTableModel *>(result.getReceiver()) != this)
        return false;

    setBusy(false);

    if(error_.isValid())
        return false;

    if(result.getError().isValid()) {
        error_ = result.getError();

        if(result.getRequestType() == QueryRequest::Select)
            emit selected(false);
        else if(result.getRequestType() == QueryRequest::CustomOperation)
            emit executed(false);
        else
            emit submitted(false);

        //qDebug() << "AsyncSqlTableModel: Error discovered.";
        submitCalled_ = false;

        return false;
    }

    switch(result.getRequestType()) {
    case QueryRequest::Select:
    {
        beginResetModel();
        records_ = result.getRecords();
        originalRecords_ = result.getRecords();
        primaryIndex_ = result.getPrimaryIndex();
        lastRecord_ = result.getLastRecord();

        // Get empty record
        emptyRecord_ = result.getRecord();

        if(!selectedSignalSuppressed_)
            emit selected(true);
        endResetModel();
    }
        break;
    case QueryRequest::Insert:
        insertedRows_.clear();
        break;
    case QueryRequest::Update:
        updatedRecordMap_.clear();
        break;
    case QueryRequest::Delete:
        removedRows_.clear();
        break;
    case QueryRequest::BeginTransaction:
        break;
    case QueryRequest::CommitTransaction:
        break;
    case QueryRequest::CustomOperation:
        emit executed(true);
        break;
    }

    if(!insertedRows_.count() && !updatedRecordMap_.count() && !removedRows_.count() && submitCalled_) {
        submitCalled_ = false;
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
    sortColumn_ = column;
    order_ = order;
}

void AsyncSqlTableModel::setTable(const QString &tableName) {
    tableName_ = tableName;
}

QString AsyncSqlTableModel::tableName() const {
    return tableName_;
}

bool AsyncSqlTableModel::isDirty() const {
    return !insertedRows_.isEmpty() || !updatedRecordMap_.isEmpty() || !removedRows_.isEmpty() ;
}

bool AsyncSqlTableModel::isDirty(const QModelIndex &index) const {
    // Check if the insert rows or removed rows contains the index's row, or the
    // updated record map contains a field that is not generated i.e. that has been edited.
    return insertedRows_.contains(index.row())
            || !updatedRecordMap_.value(index.row()).field(index.column()).isGenerated()
            || removedRows_.contains(index.row());
}

QList<QSqlRecord> AsyncSqlTableModel::insertedRecords() const {
    QList<QSqlRecord> records;

    for(auto &row : insertedRows_)
        records << records_.at(row);

    return records;
}

QMap<int, QSqlRecord> AsyncSqlTableModel::updatedRecords() const {
    return updatedRecordMap_;
}

QList<QSqlRecord> AsyncSqlTableModel::removedRecords() const {
    QList<QSqlRecord> records;

    for(auto &row : removedRows_)
        records << records_.at(row);

    return records;
}

void AsyncSqlTableModel::setLastError(const QSqlError &e) {
    error_ = e;
}

QSqlError AsyncSqlTableModel::lastError() const {
    return error_;
}

void AsyncSqlTableModel::setForeignKeyFlag(bool flag)
{
    foreignKeyFlag_ = flag;
}

bool AsyncSqlTableModel::foreignKeyFlag() const
{
    return foreignKeyFlag_;
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
    return data(index(currentRow_, fieldIndex(columnName)), Qt::DisplayRole);
}

void AsyncSqlTableModel::beginTransaction() {
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
    if(insertedRows_.isEmpty() && updatedRecordMap_.isEmpty() && removedRows_.isEmpty()) {
        emit submitted(true);
        return;
    }
    if(!validateModel())
        return;
    if(submitCalled_)
        return;

//    if(transactionState == -1) {
//        insertedRows_ = originalInsertedRows_;
//        updatedRecordMap_ = originalUpdatedRecordMap_;
//        removedRows_ = originalRemovedRows_;
//    }
//    else {
        originalUpdatedRecordMap_ = updatedRecordMap_;
        originalInsertedRows_ = insertedRows_;
        originalRemovedRows_ = removedRows_;
    //}

    qDebug() << "For " << tableName_ << ":";
    qDebug() << "What are the updated rows? " << updatedRecordMap_.keys();
    qDebug() << "What are the inserted rows? " << insertedRows_;
    qDebug() << "What are the removed rows? " << removedRows_;
    qDebug() << "-----------------------------------------------------------";

    error_ = QSqlError();

    QueryRequest request(this);
    request.setTableName(tableName_);
    request.setPrimaryIndex(primaryIndex_);
    QList<QSqlRecord> records;

    qDebug() << "Primary index? " << primaryIndex_.fieldName(0) << ", auto value: "
             << primaryIndex_.field(0).isAutoValue();

    // Update records
    if(!updatedRecordMap_.isEmpty()) {
        request.setRequestType(QueryRequest::Update);
        records = updatedRecordMap_.values();
        request.setRecords(records);
        emit execute(request);
    }


    // Insert records
    if(!insertedRows_.isEmpty()) {
        request.setRequestType(QueryRequest::Insert);
        records.clear();
        for(int i = 0; i < insertedRows_.count(); ++i) {
            qDebug() << "Inserted records: " << this->records_.at(insertedRows_.at(i));
            records << this->records_.at(insertedRows_.at(i));
        }
        request.setRecords(records);
        emit execute(request);
        records.clear();
    }


    // Delete records
    if(!removedRows_.isEmpty()) {
        request.setRequestType(QueryRequest::Delete);
        for(int i = 0; i < removedRows_.count(); ++i)
            records << this->records_.at(removedRows_.at(i));
        request.setRecords(records);
        emit execute(request);
        records.clear();
    }

    submitCalled_ = true;
}

void AsyncSqlTableModel::revertAll() {
    beginResetModel();
    records_ = originalRecords_;
    insertedRows_.clear();
    updatedRecordMap_.clear();
    removedRows_.clear();
    endResetModel();
}

QSqlRecord AsyncSqlTableModel::record() const {
    return emptyRecord_;
}

QSqlRecord AsyncSqlTableModel::record(int row) const {
    if((row < 0) || (row >= records_.count()))
        return emptyRecord_;

    return records_.at(row);
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
    this->selectedSignalSuppressed_ = s;
}

bool AsyncSqlTableModel::isSelectedSignalSuppressed() const {
    return selectedSignalSuppressed_;
}

void AsyncSqlTableModel::setRecords(const QList<QSqlRecord> &records) {
    this->records_ = records;
}

void AsyncSqlTableModel::setOriginalRecords(const QList<QSqlRecord> &records) {
    this->records_ = records;
}

QList<QSqlRecord> AsyncSqlTableModel::records() const {
    return records_;
}

QList<QSqlRecord> AsyncSqlTableModel::originalRecords() const {
    return originalRecords_;
}

QSqlRecord AsyncSqlTableModel::lastRecord() const {
    return lastRecord_;
}

void AsyncSqlTableModel::setBusy(bool b)
{
    if(busy_ == b)
        return;

    busy_ = b;
    emit busyChanged(b);
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

QList<int> AsyncSqlTableModel::insertedRows() const {
    return insertedRows_;
}

QSqlIndex AsyncSqlTableModel::primaryIndex() const
{
    return primaryIndex_;
}

bool AsyncSqlTableModel::setRecord(int row, const QSqlRecord &record) {
    if((row < 0) || (row >= rowCount()))
        return false;
    if(record.count() != emptyRecord_.count())
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

void AsyncSqlTableModel::customUpdate(const QVariantMap &values)
{
    if(values.isEmpty())
        return;
}

bool AsyncSqlTableModel::isBusy() const
{
    return busy_;
}
