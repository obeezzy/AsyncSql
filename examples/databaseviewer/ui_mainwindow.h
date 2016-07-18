/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Database_options;
    QAction *actionExit;
    QAction *actionDatabase_options;
    QAction *actionExit_2;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *submitButton;
    QPushButton *revertButton;
    QGroupBox *selectOptionsGroupBox;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *filterLineEdit;
    QLabel *label_2;
    QSpinBox *limitSpinBox;
    QGroupBox *sortOptionsGroupBox;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QSpinBox *sortColumnSpinBox;
    QLabel *label_4;
    QComboBox *sortOrderComboBox;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *selectButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(456, 515);
        action_Database_options = new QAction(MainWindow);
        action_Database_options->setObjectName(QStringLiteral("action_Database_options"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionDatabase_options = new QAction(MainWindow);
        actionDatabase_options->setObjectName(QStringLiteral("actionDatabase_options"));
        actionExit_2 = new QAction(MainWindow);
        actionExit_2->setObjectName(QStringLiteral("actionExit_2"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_5 = new QVBoxLayout(centralWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableView = new QTableView(centralWidget);
        tableView->setObjectName(QStringLiteral("tableView"));

        verticalLayout->addWidget(tableView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        submitButton = new QPushButton(centralWidget);
        submitButton->setObjectName(QStringLiteral("submitButton"));

        horizontalLayout->addWidget(submitButton);

        revertButton = new QPushButton(centralWidget);
        revertButton->setObjectName(QStringLiteral("revertButton"));

        horizontalLayout->addWidget(revertButton);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_4->addLayout(verticalLayout);

        selectOptionsGroupBox = new QGroupBox(centralWidget);
        selectOptionsGroupBox->setObjectName(QStringLiteral("selectOptionsGroupBox"));
        selectOptionsGroupBox->setCheckable(true);
        selectOptionsGroupBox->setChecked(false);
        verticalLayout_3 = new QVBoxLayout(selectOptionsGroupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(selectOptionsGroupBox);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        filterLineEdit = new QLineEdit(selectOptionsGroupBox);
        filterLineEdit->setObjectName(QStringLiteral("filterLineEdit"));

        horizontalLayout_2->addWidget(filterLineEdit);

        label_2 = new QLabel(selectOptionsGroupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        limitSpinBox = new QSpinBox(selectOptionsGroupBox);
        limitSpinBox->setObjectName(QStringLiteral("limitSpinBox"));
        limitSpinBox->setMaximum(1000000000);

        horizontalLayout_2->addWidget(limitSpinBox);


        verticalLayout_2->addLayout(horizontalLayout_2);

        sortOptionsGroupBox = new QGroupBox(selectOptionsGroupBox);
        sortOptionsGroupBox->setObjectName(QStringLiteral("sortOptionsGroupBox"));
        sortOptionsGroupBox->setCheckable(true);
        sortOptionsGroupBox->setChecked(false);
        gridLayout_2 = new QGridLayout(sortOptionsGroupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_3 = new QLabel(sortOptionsGroupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        sortColumnSpinBox = new QSpinBox(sortOptionsGroupBox);
        sortColumnSpinBox->setObjectName(QStringLiteral("sortColumnSpinBox"));
        sortColumnSpinBox->setMaximum(1000000000);

        gridLayout->addWidget(sortColumnSpinBox, 0, 1, 1, 1);

        label_4 = new QLabel(sortOptionsGroupBox);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        sortOrderComboBox = new QComboBox(sortOptionsGroupBox);
        sortOrderComboBox->setObjectName(QStringLiteral("sortOrderComboBox"));

        gridLayout->addWidget(sortOrderComboBox, 1, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        verticalLayout_2->addWidget(sortOptionsGroupBox);


        verticalLayout_3->addLayout(verticalLayout_2);


        verticalLayout_4->addWidget(selectOptionsGroupBox);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        selectButton = new QPushButton(centralWidget);
        selectButton->setObjectName(QStringLiteral("selectButton"));

        horizontalLayout_3->addWidget(selectButton);


        verticalLayout_4->addLayout(horizontalLayout_3);


        verticalLayout_5->addLayout(verticalLayout_4);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 456, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionDatabase_options);
        menuFile->addAction(actionExit_2);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        action_Database_options->setText(QApplication::translate("MainWindow", "Database options...", 0));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0));
        actionDatabase_options->setText(QApplication::translate("MainWindow", "Database options...", 0));
        actionExit_2->setText(QApplication::translate("MainWindow", "Exit", 0));
        submitButton->setText(QApplication::translate("MainWindow", "Submit changes", 0));
        revertButton->setText(QApplication::translate("MainWindow", "Revert changes", 0));
        selectOptionsGroupBox->setTitle(QApplication::translate("MainWindow", "Select options", 0));
        label->setText(QApplication::translate("MainWindow", "Filter:", 0));
        label_2->setText(QApplication::translate("MainWindow", "Limit:", 0));
        sortOptionsGroupBox->setTitle(QApplication::translate("MainWindow", "Sort options", 0));
        label_3->setText(QApplication::translate("MainWindow", "Sort column:", 0));
        label_4->setText(QApplication::translate("MainWindow", "Sort order:", 0));
        sortOrderComboBox->clear();
        sortOrderComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Ascending", 0)
         << QApplication::translate("MainWindow", "Descending", 0)
        );
        selectButton->setText(QApplication::translate("MainWindow", "Select", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
