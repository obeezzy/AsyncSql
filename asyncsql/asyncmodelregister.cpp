#include "asyncmodelregister.h"
#include "asyncsqltablemodel.h"
#include <QDebug>

using namespace AsyncSql;

AsyncModelRegister::AsyncModelRegister(QObject *parent, MarkMethod method) :
    method(method),
    totalMarked(0),
    QObject(parent)
{
}

void AsyncModelRegister::addModel(AsyncSqlTableModel *model) {
    if(modelMap.contains(model))
        return;

    modelMap.insert(model, false);

    if(method == MarkAfterSelected)
        connect(model, SIGNAL(selected(bool)), this, SLOT(mark()));
    else if(method == MarkAfterSubmitted)
        connect(model, SIGNAL(submitted(bool)), this, SLOT(mark()));
}

void AsyncModelRegister::mark() {
    AsyncSqlTableModel *model = qobject_cast<AsyncSqlTableModel *>(sender());

    if(modelMap.value(model))
        return;

    modelMap.insert(model, true);
    totalMarked++;

    if(isDone())
        emit allMarked();
}

bool AsyncModelRegister::removeModel(AsyncSqlTableModel *model) {
    if(!modelMap.remove(model))
        return false;

    if(totalMarked > 0)
        totalMarked--;

    disconnect(model, SIGNAL(selected(bool)), this, SLOT(mark()));
    disconnect(model, SIGNAL(submitted(bool)), this, SLOT(mark()));

    return true;
}

bool AsyncModelRegister::removeAllModels() {
    if(modelMap.isEmpty())
        return false;

    for(auto &model : modelMap.keys())
        removeModel(model);

    return true;
}

void AsyncModelRegister::clear() {
    for(int i = 0; i < modelMap.count(); ++i) {
        AsyncSqlTableModel *model = modelMap.keys().at(i);

        modelMap.insert(model, false);
    }

    totalMarked = 0;
}

QList<AsyncSqlTableModel *> AsyncModelRegister::getModels() const {
    return modelMap.keys();
}

bool AsyncModelRegister::isDone() const {
    return modelMap.count() == totalMarked;
}
