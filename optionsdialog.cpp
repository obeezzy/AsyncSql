#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "asyncsql/databaseconnection.h"
#include <QFileDialog>
#include <QStandardPaths>

using namespace AsyncSql;

QString OptionsDialog::tableName;

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);

    load();
    setWindowTitle(tr("Choose database options"));

    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(reject()));

    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(browse()));
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::setTableName(const QString &name)
{
    tableName = name;
}

QString OptionsDialog::getTableName()
{
    return tableName;
}

void OptionsDialog::load()
{
    ui->hostNameLineEdit->setText(DatabaseConnection::getDefaultHostName());
    ui->portSpinBox->setValue(DatabaseConnection::getDefaultPort());
    ui->userNameLineEdit->setText(DatabaseConnection::getDefaultUserName());
    ui->passwordLineEdit->setText(DatabaseConnection::getDefaultPassword());
    ui->databaseNameLineEdit->setText(DatabaseConnection::getDefaultDatabaseName());
    ui->connectOptionsLineEdit->setText(DatabaseConnection::getDefaultConnectOptions());
    ui->tableNameLineEdit->setText(tableName);

    if(DatabaseConnection::getDefaultDriver() == DatabaseConnection::SQLite)
        ui->sqlDriverComboBox->setCurrentIndex(0);
    else if(DatabaseConnection::getDefaultDriver() == DatabaseConnection::MySQL)
        ui->sqlDriverComboBox->setCurrentIndex(1);
}

void OptionsDialog::save()
{
    DatabaseConnection::setDefaultHostName(ui->hostNameLineEdit->text().trimmed());
    DatabaseConnection::setDefaultPort(ui->portSpinBox->value());
    DatabaseConnection::setDefaultUserName(ui->userNameLineEdit->text().trimmed());
    DatabaseConnection::setDefaultPassword(ui->passwordLineEdit->text().trimmed());
    DatabaseConnection::setDefaultDatabaseName(ui->databaseNameLineEdit->text().trimmed());
    DatabaseConnection::setDefaultConnectOptions(ui->connectOptionsLineEdit->text().trimmed());
    tableName = ui->tableNameLineEdit->text().trimmed();

    if(ui->sqlDriverComboBox->currentText().trimmed() == "SQLite")
        DatabaseConnection::setDefaultDriver(DatabaseConnection::SQLite);
    else if(ui->sqlDriverComboBox->currentText().trimmed() == "MySQL")
        DatabaseConnection::setDefaultDriver(DatabaseConnection::MySQL);

    accept();
}

void OptionsDialog::browse()
{
    ui->databaseNameLineEdit->setText(
                QFileDialog::getOpenFileName(this, tr("Open File"),
                                             QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                             tr("Databases (*.db *.sqlite *.sqlite.db)")));
}
