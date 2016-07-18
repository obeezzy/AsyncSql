/********************************************************************************
** Form generated from reading UI file 'optionsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONSDIALOG_H
#define UI_OPTIONSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_OptionsDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *portSpinBox;
    QLineEdit *hostNameLineEdit;
    QLabel *label_2;
    QLabel *label_6;
    QLineEdit *userNameLineEdit;
    QComboBox *sqlDriverComboBox;
    QLabel *label_4;
    QLineEdit *connectOptionsLineEdit;
    QLabel *label_3;
    QLineEdit *passwordLineEdit;
    QLabel *label_5;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout;
    QLineEdit *databaseNameLineEdit;
    QPushButton *browseButton;
    QLineEdit *tableNameLineEdit;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *saveButton;
    QPushButton *closeButton;

    void setupUi(QDialog *OptionsDialog)
    {
        if (OptionsDialog->objectName().isEmpty())
            OptionsDialog->setObjectName(QStringLiteral("OptionsDialog"));
        OptionsDialog->resize(423, 260);
        verticalLayout_2 = new QVBoxLayout(OptionsDialog);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(OptionsDialog);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        portSpinBox = new QSpinBox(OptionsDialog);
        portSpinBox->setObjectName(QStringLiteral("portSpinBox"));
        portSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        portSpinBox->setMaximum(65535);

        gridLayout->addWidget(portSpinBox, 1, 1, 1, 1);

        hostNameLineEdit = new QLineEdit(OptionsDialog);
        hostNameLineEdit->setObjectName(QStringLiteral("hostNameLineEdit"));

        gridLayout->addWidget(hostNameLineEdit, 0, 1, 1, 1);

        label_2 = new QLabel(OptionsDialog);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label_6 = new QLabel(OptionsDialog);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 1, 0, 1, 1);

        userNameLineEdit = new QLineEdit(OptionsDialog);
        userNameLineEdit->setObjectName(QStringLiteral("userNameLineEdit"));

        gridLayout->addWidget(userNameLineEdit, 2, 1, 1, 1);

        sqlDriverComboBox = new QComboBox(OptionsDialog);
        sqlDriverComboBox->setObjectName(QStringLiteral("sqlDriverComboBox"));

        gridLayout->addWidget(sqlDriverComboBox, 4, 1, 1, 1);

        label_4 = new QLabel(OptionsDialog);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        connectOptionsLineEdit = new QLineEdit(OptionsDialog);
        connectOptionsLineEdit->setObjectName(QStringLiteral("connectOptionsLineEdit"));

        gridLayout->addWidget(connectOptionsLineEdit, 5, 1, 1, 1);

        label_3 = new QLabel(OptionsDialog);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        passwordLineEdit = new QLineEdit(OptionsDialog);
        passwordLineEdit->setObjectName(QStringLiteral("passwordLineEdit"));
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(passwordLineEdit, 3, 1, 1, 1);

        label_5 = new QLabel(OptionsDialog);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 7, 0, 1, 1);

        label_7 = new QLabel(OptionsDialog);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 5, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        databaseNameLineEdit = new QLineEdit(OptionsDialog);
        databaseNameLineEdit->setObjectName(QStringLiteral("databaseNameLineEdit"));

        horizontalLayout->addWidget(databaseNameLineEdit);

        browseButton = new QPushButton(OptionsDialog);
        browseButton->setObjectName(QStringLiteral("browseButton"));

        horizontalLayout->addWidget(browseButton);


        gridLayout->addLayout(horizontalLayout, 7, 1, 1, 1);

        tableNameLineEdit = new QLineEdit(OptionsDialog);
        tableNameLineEdit->setObjectName(QStringLiteral("tableNameLineEdit"));

        gridLayout->addWidget(tableNameLineEdit, 6, 1, 1, 1);

        label_8 = new QLabel(OptionsDialog);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 6, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        saveButton = new QPushButton(OptionsDialog);
        saveButton->setObjectName(QStringLiteral("saveButton"));

        horizontalLayout_2->addWidget(saveButton);

        closeButton = new QPushButton(OptionsDialog);
        closeButton->setObjectName(QStringLiteral("closeButton"));

        horizontalLayout_2->addWidget(closeButton);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);

        QWidget::setTabOrder(hostNameLineEdit, portSpinBox);
        QWidget::setTabOrder(portSpinBox, userNameLineEdit);
        QWidget::setTabOrder(userNameLineEdit, passwordLineEdit);
        QWidget::setTabOrder(passwordLineEdit, sqlDriverComboBox);
        QWidget::setTabOrder(sqlDriverComboBox, connectOptionsLineEdit);
        QWidget::setTabOrder(connectOptionsLineEdit, tableNameLineEdit);
        QWidget::setTabOrder(tableNameLineEdit, databaseNameLineEdit);
        QWidget::setTabOrder(databaseNameLineEdit, browseButton);
        QWidget::setTabOrder(browseButton, saveButton);
        QWidget::setTabOrder(saveButton, closeButton);

        retranslateUi(OptionsDialog);

        QMetaObject::connectSlotsByName(OptionsDialog);
    } // setupUi

    void retranslateUi(QDialog *OptionsDialog)
    {
        OptionsDialog->setWindowTitle(QApplication::translate("OptionsDialog", "Dialog", 0));
        label->setText(QApplication::translate("OptionsDialog", "Host name:", 0));
        hostNameLineEdit->setText(QString());
        hostNameLineEdit->setPlaceholderText(QApplication::translate("OptionsDialog", "Enter host name", 0));
        label_2->setText(QApplication::translate("OptionsDialog", "User name:", 0));
        label_6->setText(QApplication::translate("OptionsDialog", "Port:", 0));
        userNameLineEdit->setPlaceholderText(QApplication::translate("OptionsDialog", "Enter user name", 0));
        sqlDriverComboBox->clear();
        sqlDriverComboBox->insertItems(0, QStringList()
         << QApplication::translate("OptionsDialog", "SQLite", 0)
         << QApplication::translate("OptionsDialog", "MySQL", 0)
        );
        label_4->setText(QApplication::translate("OptionsDialog", "SQL Driver:", 0));
        connectOptionsLineEdit->setPlaceholderText(QApplication::translate("OptionsDialog", "Set connect options", 0));
        label_3->setText(QApplication::translate("OptionsDialog", "Password:", 0));
        passwordLineEdit->setPlaceholderText(QApplication::translate("OptionsDialog", "Enter password", 0));
        label_5->setText(QApplication::translate("OptionsDialog", "Database name:", 0));
        label_7->setText(QApplication::translate("OptionsDialog", "Connect options:", 0));
        databaseNameLineEdit->setPlaceholderText(QApplication::translate("OptionsDialog", "Database name", 0));
        browseButton->setText(QApplication::translate("OptionsDialog", "Browse...", 0));
        tableNameLineEdit->setPlaceholderText(QApplication::translate("OptionsDialog", "Enter table name", 0));
        label_8->setText(QApplication::translate("OptionsDialog", "Table name:", 0));
        saveButton->setText(QApplication::translate("OptionsDialog", "Save", 0));
        closeButton->setText(QApplication::translate("OptionsDialog", "Close", 0));
    } // retranslateUi

};

namespace Ui {
    class OptionsDialog: public Ui_OptionsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONSDIALOG_H
