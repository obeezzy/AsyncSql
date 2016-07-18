#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "asyncsql/asyncsqltablemodel.h"
#include "asyncsql/databaseconnection.h"
#include <QMessageBox>
#include "optionsdialog.h"

using namespace AsyncSql;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DatabaseConnection::setDefaultDatabaseName("databases/example.db");
    DatabaseConnection::setDefaultDriver(DatabaseConnection::SQLite);
    OptionsDialog::setTableName("sales");

    model = new AsyncSqlTableModel(this);
    model->setTable(OptionsDialog::getTableName());
    ui->tableView->setModel(model);

    connect(ui->actionDatabase_options, SIGNAL(triggered()), this, SLOT(showOptions()));
    connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(ui->selectButton, SIGNAL(clicked()), this, SLOT(select()));
    connect(model, SIGNAL(selected(bool)), this, SLOT(onSelected(bool)));
    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(submit()));
    connect(model, SIGNAL(submitted(bool)), this, SLOT(onSubmitted(bool)));
    connect(ui->revertButton, SIGNAL(clicked()), this, SLOT(revert()));

    setWindowTitle("Async SQL");
}

void MainWindow::select()
{
    if(ui->selectOptionsGroupBox->isChecked())
    {
        model->setFilter(ui->filterLineEdit->text().trimmed());
        model->setLimit(ui->limitSpinBox->value());

        if(ui->sortOptionsGroupBox->isChecked())
        {
            Qt::SortOrder order = ui->sortOrderComboBox->currentText() == "Ascending" ?
                        Qt::AscendingOrder : Qt::DescendingOrder;

            model->setSort(ui->sortColumnSpinBox->value(), order);
        }
    }
    else
    {
        // Reset everything
        model->setFilter("");
        model->setLimit(-1);
        model->setSort(-1, Qt::AscendingOrder);
    }

    model->select();
    ui->statusBar->showMessage(tr("Selecting..."));
}

void MainWindow::onSelected(bool successful)
{
    if(successful)
        ui->statusBar->showMessage(tr("Selecting done!"), 5000);
    else
        QMessageBox::critical(this, tr("Model status"), tr("Error: ") + model->lastError().text());
}

void MainWindow::submit()
{
    if(!model->isDirty())
    {
        ui->statusBar->showMessage(tr("No changes made."), 5000);
    }
    else
    {
        model->submitAll();
        ui->statusBar->showMessage(tr("Submitting..."));
    }
}

void MainWindow::onSubmitted(bool successful)
{
    if(successful)
        ui->statusBar->showMessage(tr("Submitting done!"), 5000);
    else
        QMessageBox::critical(this, tr("Model status"), tr("Error: ") + model->lastError().text());
}

void MainWindow::revert()
{
    if(!model->isDirty())
    {
        ui->statusBar->showMessage(tr("Nothing to revert."), 5000);
    }
    else
    {
        model->revertAll();
        ui->statusBar->showMessage(tr("Reverted changes!"), 5000);
    }
}

void MainWindow::showOptions()
{
    OptionsDialog dialog(this);

    if(dialog.exec())
    {
        model->setTable(OptionsDialog::getTableName());
        model->select();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
