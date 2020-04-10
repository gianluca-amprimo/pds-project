/********************************************************************************
** Form generated from reading UI file 'registrationwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTRATIONWINDOW_H
#define UI_REGISTRATIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
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
    QLabel *RepeatPasswordLabel;
    QLabel *PasswordLabel;
    QLineEdit *UsernameEdit;
    QLabel *NameLabel;
    QLabel *UsernameLabel;
    QLineEdit *RepeatPasswordEdit;
    QLineEdit *SurnameEdit;
    QLineEdit *PasswordEdit;
    QLabel *SurnameLabel;
    QLineEdit *NameEdit;
    QLabel *ProfilePicture;
    QVBoxLayout *verticalLayout_2;
    QPushButton *ProfilePictureButton;
    QPushButton *DeletePictureButton;
    QHBoxLayout *horizontalLayout;
    QPushButton *RegisterButton;
    QFrame *line;

    void setupUi(QDialog *RegistrationWindow)
    {
        if (RegistrationWindow->objectName().isEmpty())
            RegistrationWindow->setObjectName(QStringLiteral("RegistrationWindow"));
        RegistrationWindow->resize(329, 294);
        verticalLayout = new QVBoxLayout(RegistrationWindow);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        RepeatPasswordLabel = new QLabel(RegistrationWindow);
        RepeatPasswordLabel->setObjectName(QStringLiteral("RepeatPasswordLabel"));

        gridLayout->addWidget(RepeatPasswordLabel, 5, 0, 1, 1);

        PasswordLabel = new QLabel(RegistrationWindow);
        PasswordLabel->setObjectName(QStringLiteral("PasswordLabel"));

        gridLayout->addWidget(PasswordLabel, 4, 0, 1, 1);

        UsernameEdit = new QLineEdit(RegistrationWindow);
        UsernameEdit->setObjectName(QStringLiteral("UsernameEdit"));

        gridLayout->addWidget(UsernameEdit, 3, 1, 1, 1);

        NameLabel = new QLabel(RegistrationWindow);
        NameLabel->setObjectName(QStringLiteral("NameLabel"));

        gridLayout->addWidget(NameLabel, 1, 0, 1, 1);

        UsernameLabel = new QLabel(RegistrationWindow);
        UsernameLabel->setObjectName(QStringLiteral("UsernameLabel"));

        gridLayout->addWidget(UsernameLabel, 3, 0, 1, 1);

        RepeatPasswordEdit = new QLineEdit(RegistrationWindow);
        RepeatPasswordEdit->setObjectName(QStringLiteral("RepeatPasswordEdit"));
        RepeatPasswordEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(RepeatPasswordEdit, 5, 1, 1, 1);

        SurnameEdit = new QLineEdit(RegistrationWindow);
        SurnameEdit->setObjectName(QStringLiteral("SurnameEdit"));

        gridLayout->addWidget(SurnameEdit, 2, 1, 1, 1);

        PasswordEdit = new QLineEdit(RegistrationWindow);
        PasswordEdit->setObjectName(QStringLiteral("PasswordEdit"));
        PasswordEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(PasswordEdit, 4, 1, 1, 1);

        SurnameLabel = new QLabel(RegistrationWindow);
        SurnameLabel->setObjectName(QStringLiteral("SurnameLabel"));

        gridLayout->addWidget(SurnameLabel, 2, 0, 1, 1);

        NameEdit = new QLineEdit(RegistrationWindow);
        NameEdit->setObjectName(QStringLiteral("NameEdit"));

        gridLayout->addWidget(NameEdit, 1, 1, 1, 1);

        ProfilePicture = new QLabel(RegistrationWindow);
        ProfilePicture->setObjectName(QStringLiteral("ProfilePicture"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ProfilePicture->sizePolicy().hasHeightForWidth());
        ProfilePicture->setSizePolicy(sizePolicy);
        ProfilePicture->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(ProfilePicture, 0, 0, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        ProfilePictureButton = new QPushButton(RegistrationWindow);
        ProfilePictureButton->setObjectName(QStringLiteral("ProfilePictureButton"));

        verticalLayout_2->addWidget(ProfilePictureButton);

        DeletePictureButton = new QPushButton(RegistrationWindow);
        DeletePictureButton->setObjectName(QStringLiteral("DeletePictureButton"));
        DeletePictureButton->setEnabled(false);

        verticalLayout_2->addWidget(DeletePictureButton);


        gridLayout->addLayout(verticalLayout_2, 0, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(230, -1, -1, -1);
        RegisterButton = new QPushButton(RegistrationWindow);
        RegisterButton->setObjectName(QStringLiteral("RegisterButton"));
        RegisterButton->setEnabled(false);
        RegisterButton->setMaximumSize(QSize(100, 25));

        horizontalLayout->addWidget(RegisterButton);


        verticalLayout->addLayout(horizontalLayout);

        line = new QFrame(RegistrationWindow);
        line->setObjectName(QStringLiteral("line"));
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
        RegistrationWindow->setWindowTitle(QApplication::translate("RegistrationWindow", "Registration", Q_NULLPTR));
        RepeatPasswordLabel->setText(QApplication::translate("RegistrationWindow", "Repeat password", Q_NULLPTR));
        PasswordLabel->setText(QApplication::translate("RegistrationWindow", "Password", Q_NULLPTR));
        NameLabel->setText(QApplication::translate("RegistrationWindow", "Name", Q_NULLPTR));
        UsernameLabel->setText(QApplication::translate("RegistrationWindow", "Username", Q_NULLPTR));
        SurnameLabel->setText(QApplication::translate("RegistrationWindow", "Surname", Q_NULLPTR));
        ProfilePicture->setText(QString());
        ProfilePictureButton->setText(QApplication::translate("RegistrationWindow", "Profile picture...", Q_NULLPTR));
        DeletePictureButton->setText(QApplication::translate("RegistrationWindow", "Delete picture", Q_NULLPTR));
        RegisterButton->setText(QApplication::translate("RegistrationWindow", "Register", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class RegistrationWindow: public Ui_RegistrationWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTRATIONWINDOW_H
