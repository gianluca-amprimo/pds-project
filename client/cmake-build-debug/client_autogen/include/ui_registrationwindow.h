/********************************************************************************
** Form generated from reading UI file 'registrationwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTRATIONWINDOW_H
#define UI_REGISTRATIONWINDOW_H

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

class Ui_RegistrationWindow
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *UsernameLabel;
    QLabel *PasswordLabel;
    QLineEdit *UsernameEdit;
    QLabel *SurnameLabel;
    QLineEdit *PasswordEdit;
    QLineEdit *RepeatPasswordEdit;
    QLabel *RepeatPasswordLabel;
    QLineEdit *SurnameEdit;
    QLabel *NameLabel;
    QLineEdit *NameEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *RegisterButton;
    QFrame *line;

    void setupUi(QDialog *RegistrationWindow)
    {
        if (RegistrationWindow->objectName().isEmpty())
            RegistrationWindow->setObjectName(QString::fromUtf8("RegistrationWindow"));
        RegistrationWindow->resize(329, 211);
        verticalLayout = new QVBoxLayout(RegistrationWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        UsernameLabel = new QLabel(RegistrationWindow);
        UsernameLabel->setObjectName(QString::fromUtf8("UsernameLabel"));

        gridLayout->addWidget(UsernameLabel, 2, 0, 1, 1);

        PasswordLabel = new QLabel(RegistrationWindow);
        PasswordLabel->setObjectName(QString::fromUtf8("PasswordLabel"));

        gridLayout->addWidget(PasswordLabel, 3, 0, 1, 1);

        UsernameEdit = new QLineEdit(RegistrationWindow);
        UsernameEdit->setObjectName(QString::fromUtf8("UsernameEdit"));

        gridLayout->addWidget(UsernameEdit, 2, 1, 1, 1);

        SurnameLabel = new QLabel(RegistrationWindow);
        SurnameLabel->setObjectName(QString::fromUtf8("SurnameLabel"));

        gridLayout->addWidget(SurnameLabel, 1, 0, 1, 1);

        PasswordEdit = new QLineEdit(RegistrationWindow);
        PasswordEdit->setObjectName(QString::fromUtf8("PasswordEdit"));
        PasswordEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(PasswordEdit, 3, 1, 1, 1);

        RepeatPasswordEdit = new QLineEdit(RegistrationWindow);
        RepeatPasswordEdit->setObjectName(QString::fromUtf8("RepeatPasswordEdit"));
        RepeatPasswordEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(RepeatPasswordEdit, 4, 1, 1, 1);

        RepeatPasswordLabel = new QLabel(RegistrationWindow);
        RepeatPasswordLabel->setObjectName(QString::fromUtf8("RepeatPasswordLabel"));

        gridLayout->addWidget(RepeatPasswordLabel, 4, 0, 1, 1);

        SurnameEdit = new QLineEdit(RegistrationWindow);
        SurnameEdit->setObjectName(QString::fromUtf8("SurnameEdit"));

        gridLayout->addWidget(SurnameEdit, 1, 1, 1, 1);

        NameLabel = new QLabel(RegistrationWindow);
        NameLabel->setObjectName(QString::fromUtf8("NameLabel"));

        gridLayout->addWidget(NameLabel, 0, 0, 1, 1);

        NameEdit = new QLineEdit(RegistrationWindow);
        NameEdit->setObjectName(QString::fromUtf8("NameEdit"));

        gridLayout->addWidget(NameEdit, 0, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(230, -1, -1, -1);
        RegisterButton = new QPushButton(RegistrationWindow);
        RegisterButton->setObjectName(QString::fromUtf8("RegisterButton"));
        RegisterButton->setEnabled(false);
        RegisterButton->setMaximumSize(QSize(100, 25));

        horizontalLayout->addWidget(RegisterButton);


        verticalLayout->addLayout(horizontalLayout);

        line = new QFrame(RegistrationWindow);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        QWidget::setTabOrder(NameEdit, SurnameEdit);
        QWidget::setTabOrder(SurnameEdit, UsernameEdit);
        QWidget::setTabOrder(UsernameEdit, PasswordEdit);
        QWidget::setTabOrder(PasswordEdit, RepeatPasswordEdit);
        QWidget::setTabOrder(RepeatPasswordEdit, RegisterButton);

        retranslateUi(RegistrationWindow);

        RegisterButton->setDefault(true);


        QMetaObject::connectSlotsByName(RegistrationWindow);
    } // setupUi

    void retranslateUi(QDialog *RegistrationWindow)
    {
        RegistrationWindow->setWindowTitle(QCoreApplication::translate("RegistrationWindow", "Registration", nullptr));
        UsernameLabel->setText(QCoreApplication::translate("RegistrationWindow", "Username", nullptr));
        PasswordLabel->setText(QCoreApplication::translate("RegistrationWindow", "Password", nullptr));
        SurnameLabel->setText(QCoreApplication::translate("RegistrationWindow", "Surname", nullptr));
        RepeatPasswordLabel->setText(QCoreApplication::translate("RegistrationWindow", "Repeat password", nullptr));
        NameLabel->setText(QCoreApplication::translate("RegistrationWindow", "Name", nullptr));
        RegisterButton->setText(QCoreApplication::translate("RegistrationWindow", "Register", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegistrationWindow: public Ui_RegistrationWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTRATIONWINDOW_H
