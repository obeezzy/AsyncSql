#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H

#include <exception>
#include <QString>
#include <QSqlError>

namespace AsyncSql {
class DatabaseException : std::exception
{
public:
    enum ErrorCode {FailedToCommit = 10000};
    DatabaseException(const QSqlError &e, const QString userMsg = "") :
        error(e),
        code(e.number()),
        msg(e.text())
    {
        if(userMsg.isEmpty())
            this->userMsg = msg;
    }

    const char *what() const throw() {
        return QString(QStringLiteral("Error ") + QString::number(code) + QStringLiteral(": ") + msg)
                .toStdString().c_str();
    }

    QSqlError getError() const { return error; }
    ~DatabaseException() throw() {}
private:
    int code;
    QString msg;
    QString userMsg;
    QSqlError error;
};
}

#endif // DATABASEEXCEPTION_H
