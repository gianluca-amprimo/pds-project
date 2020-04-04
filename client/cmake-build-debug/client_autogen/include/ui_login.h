/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Client
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *userLE;
    QLabel *label_2;
    QLineEdit *passwordLE;
    QPushButton *logButton;
    QTextEdit *ResultTE;
    QStatusBar *statusbar;

    void setupUi(QDialog *LOGIN)
    {
        if (LOGIN->objectName().isEmpty())
            LOGIN->setObjectName(QStringLiteral("LOGIN"));
        LOGIN->resize(530, 368);
        centralwidget = new QWidget(LOGIN);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(20, 10, 481, 301));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setTextFormat(Qt::RichText);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        userLE = new QLineEdit(verticalLayoutWidget);
        userLE->setObjectName(QStringLiteral("userLE"));

        verticalLayout->addWidget(userLE);

        label_2 = new QLabel(verticalLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setTextFormat(Qt::RichText);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        passwordLE = new QLineEdit(verticalLayoutWidget);
        passwordLE->setObjectName(QStringLiteral("passwordLE"));

        verticalLayout->addWidget(passwordLE);

        logButton = new QPushButton(verticalLayoutWidget);
        logButton->setObjectName(QStringLiteral("logButton"));

        verticalLayout->addWidget(logButton);

        ResultTE = new QTextEdit(verticalLayoutWidget);
        ResultTE->setObjectName(QStringLiteral("ResultTE"));

        verticalLayout->addWidget(ResultTE);

        statusbar = new QStatusBar(LOGIN);
        statusbar->setObjectName(QStringLiteral("statusbar"));

        retranslateUi(LOGIN);

        QMetaObject::connectSlotsByName(LOGIN);
    } // setupUi

    void retranslateUi(QDialog *LOGIN)
    {
        LOGIN->setWindowTitle(QApplication::translate("Client", "MainWindow", Q_NULLPTR));
        label->setText(QApplication::translate("Client", "Username", Q_NULLPTR));
        label_2->setText(QApplication::translate("Client", "Password", Q_NULLPTR));
        logButton->setText(QApplication::translate("Client", "Log in", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Client: public Ui_Client {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
