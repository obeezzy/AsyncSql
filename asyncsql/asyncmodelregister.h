#ifndef ASYNCMODELREGISTER_H
#define ASYNCMODELREGISTER_H

#include <QObject>
#include <QMap>

namespace AsyncSql {
class AsyncSqlTableModel;

class AsyncModelRegister : public QObject
{
    Q_OBJECT
public:
    enum MarkMethod {MarkAfterSelected, MarkAfterSubmitted};
    explicit AsyncModelRegister(QObject *parent = 0, MarkMethod method = MarkAfterSelected);

    void addModel(AsyncSqlTableModel *model);
    bool removeModel(AsyncSqlTableModel *model);
    bool removeAllModels();
    bool isDone() const;

    QList<AsyncSqlTableModel *> getModels() const;
signals:
    void allMarked();
public slots:
    void mark();
    void clear();
private:
    QMap<AsyncSqlTableModel *, bool> modelMap;
    MarkMethod method;
    int totalMarked;
};
}

#endif // ASYNCMODELREGISTER_H
