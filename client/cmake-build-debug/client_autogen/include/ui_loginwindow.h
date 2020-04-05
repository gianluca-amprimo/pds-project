/********************************************************************************
** Form generated from reading UI file 'loginwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWINDOW_H
#define UI_LOGINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *UsernameLabel;
    QLineEdit *UsernameEdit;
    QLabel *PasswordLabel;
    QLineEdit *PasswordEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *CancelButton;
    QPushButton *LoginButton;
    QLabel *RegistrationLink;
    QFrame *line;

    void setupUi(QDialog *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName(QString::fromUtf8("LoginWindow"));
        LoginWindow->resize(288, 141);
        verticalLayout = new QVBoxLayout(LoginWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        UsernameLabel = new QLabel(LoginWindow);
        UsernameLabel->setObjectName(QString::fromUtf8("UsernameLabel"));

        gridLayout->addWidget(UsernameLabel, 0, 0, 1, 1);

        UsernameEdit = new QLineEdit(LoginWindow);
        UsernameEdit->setObjectName(QString::fromUtf8("UsernameEdit"));

        gridLayout->addWidget(UsernameEdit, 0, 1, 1, 1);

        PasswordLabel = new QLabel(LoginWindow);
        PasswordLabel->setObjectName(QString::fromUtf8("PasswordLabel"));

        gridLayout->addWidget(PasswordLabel, 1, 0, 1, 1);

        PasswordEdit = new QLineEdit(LoginWindow);
        PasswordEdit->setObjectName(QString::fromUtf8("PasswordEdit"));
        PasswordEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(PasswordEdit, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(100, -1, -1, -1);
        CancelButton = new QPushButton(LoginWindow);
        CancelButton->setObjectName(QString::fromUtf8("CancelButton"));
        CancelButton->setMaximumSize(QSize(100, 25));
        CancelButton->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout->addWidget(CancelButton);

        LoginButton = new QPushButton(LoginWindow);
        LoginButton->setObjectName(QString::fromUtf8("LoginButton"));
        LoginButton->setEnabled(true);
        LoginButton->setMaximumSize(QSize(100, 25));
        LoginButton->setMouseTracking(false);
        LoginButton->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout->addWidget(LoginButton);


        verticalLayout->addLayout(horizontalLayout);

        RegistrationLink = new QLabel(LoginWindow);
        RegistrationLink->setObjectName(QString::fromUtf8("RegistrationLink"));
        RegistrationLink->setCursor(QCursor(Qt::PointingHandCursor));
        RegistrationLink->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

        verticalLayout->addWidget(RegistrationLink);

        line = new QFrame(LoginWindow);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);


        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QDialog *LoginWindow)
    {
        LoginWindow->setWindowTitle(QApplication::translate("LoginWindow", "Login", nullptr));
        UsernameLabel->setText(QApplication::translate("LoginWindow", "Username", nullptr));
        PasswordLabel->setText(QApplication::translate("LoginWindow", "Password", nullptr));
        PasswordEdit->setText(QString());
        CancelButton->setText(QApplication::translate("LoginWindow", "Cancel", nullptr));
        LoginButton->setText(QApplication::translate("LoginWindow", "Login", nullptr));
        RegistrationLink->setText(QApplication::translate("LoginWindow", "<html><head/><body><p><span style=\" text-decoration: underline; color:#0000ff;\">Are you not registered yet?</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWINDOW_H
