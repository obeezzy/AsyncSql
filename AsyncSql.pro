#-------------------------------------------------
#
# Project created by QtCreator 2016-02-22T02:57:22
#
#-------------------------------------------------

QT       += core gui sql

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AsyncSql
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    asyncsql/asyncmodelregister.cpp \
    asyncsql/asyncsqltablemodel.cpp \
    asyncsql/databaseconnection.cpp \
    asyncsql/queryrequest.cpp \
    asyncsql/queryresult.cpp \
    asyncsql/querythread.cpp \
    asyncsql/queryworker.cpp \
    asyncsql/databaseexception.cpp \
    optionsdialog.cpp

HEADERS  += mainwindow.h \
    asyncsql/asyncmodelregister.h \
    asyncsql/asyncsqltablemodel.h \
    asyncsql/databaseconnection.h \
    asyncsql/queryrequest.h \
    asyncsql/queryresult.h \
    asyncsql/querythread.h \
    asyncsql/queryworker.h \
    asyncsql/databaseexception.h \
    optionsdialog.h

FORMS    += mainwindow.ui \
    optionsdialog.ui
