#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "asyncsql/asyncsqltablemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void select();
    void onSelected(bool successful);
    void submit();
    void onSubmitted(bool successful);
    void revert();
    void showOptions();
private:
    Ui::MainWindow *ui;
    AsyncSql::AsyncSqlTableModel *model;
};

#endif // MAINWINDOW_H
