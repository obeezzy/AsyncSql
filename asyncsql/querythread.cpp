#include "querythread.h"
#include "queryworker.h"
#include <QDebug>
#include "queryrequest.h"
#include "queryresult.h"
#include <QCoreApplication>

using namespace AsyncSql;

QueryThread *QueryThread::this_thread = nullptr;
QueryThread::TransactionState QueryThread::transactionState = QueryThread::Unset;

QueryThread::QueryThread(QObject *parent) :
    QThread(parent)
{
}

const QueryThread &QueryThread::instance()
{
    if(!this_thread)
    {
        this_thread = new QueryThread();
        connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), this_thread, SLOT(quit()));
        this_thread->start();
    }

    return *this_thread;
}

void QueryThread::run() {
    qDebug() << "Query thread started.";
    // Create worker object within the context of the new thread
    m_worker = new QueryWorker();

    // forward to the worker: a 'queued connection'!
    connect(this, SIGNAL(queue(const QueryRequest &)),
            m_worker, SLOT(execute(const QueryRequest &)));

    // first, make the object system aware
    qRegisterMetaType<QueryRequest>("QueryRequest");
    qRegisterMetaType<QueryResult>("QueryResult");
    // now set up the queued connection
    // forward a signal back out
    connect(m_worker, SIGNAL(resultsReady(const QueryResult &)),
            this, SIGNAL(queryFinished(const QueryResult &)));

    exec(); // start our own event loop
}

void QueryThread::execute(const QueryRequest &request)
{
    qDebug() << "QueryThread::execute() got this query: " << request.getQuery();
    emit queue(request); // queues to worker
}

QString QueryThread::getSqliteConnectionName()
{
    return QueryWorker::getSqliteConnectionName();
}

QString QueryThread::getMysqlConnectionName()
{
    return QueryWorker::getMysqlConnectionName();
}

void QueryThread::setTransactionState(QueryThread::TransactionState state)
{
    transactionState = state;
}

QueryThread::TransactionState QueryThread::getTransactionState()
{
    return transactionState;
}
