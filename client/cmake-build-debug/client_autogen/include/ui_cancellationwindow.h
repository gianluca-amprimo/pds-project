/********************************************************************************
** Form generated from reading UI file 'cancellationwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANCELLATIONWINDOW_H
#define UI_CANCELLATIONWINDOW_H

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

class Ui_CancellationWindow
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *UsernameLabel;
    QLineEdit *UsernameEdit;
    QLabel *PasswordLabel;
    QLineEdit *PasswordEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *DeleteButton;
    QFrame *line;

    void setupUi(QDialog *CancellationWindow)
    {
        if (CancellationWindow->objectName().isEmpty())
            CancellationWindow->setObjectName(QString::fromUtf8("CancellationWindow"));
        CancellationWindow->resize(283, 120);
        verticalLayout = new QVBoxLayout(CancellationWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        UsernameLabel = new QLabel(CancellationWindow);
        UsernameLabel->setObjectName(QString::fromUtf8("UsernameLabel"));

        gridLayout->addWidget(UsernameLabel, 0, 0, 1, 1);

        UsernameEdit = new QLineEdit(CancellationWindow);
        UsernameEdit->setObjectName(QString::fromUtf8("UsernameEdit"));

        gridLayout->addWidget(UsernameEdit, 0, 1, 1, 1);

        PasswordLabel = new QLabel(CancellationWindow);
        PasswordLabel->setObjectName(QString::fromUtf8("PasswordLabel"));

        gridLayout->addWidget(PasswordLabel, 1, 0, 1, 1);

        PasswordEdit = new QLineEdit(CancellationWindow);
        PasswordEdit->setObjectName(QString::fromUtf8("PasswordEdit"));
        PasswordEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(PasswordEdit, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(170, -1, -1, -1);
        DeleteButton = new QPushButton(CancellationWindow);
        DeleteButton->setObjectName(QString::fromUtf8("DeleteButton"));
        DeleteButton->setEnabled(false);
        DeleteButton->setMaximumSize(QSize(100, 25));
        DeleteButton->setMouseTracking(false);
        DeleteButton->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout->addWidget(DeleteButton);


        verticalLayout->addLayout(horizontalLayout);

        line = new QFrame(CancellationWindow);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);


        retranslateUi(CancellationWindow);

        DeleteButton->setDefault(true);


        QMetaObject::connectSlotsByName(CancellationWindow);
    } // setupUi

    void retranslateUi(QDialog *CancellationWindow)
    {
        CancellationWindow->setWindowTitle(QCoreApplication::translate("CancellationWindow", "Cancellation", nullptr));
        UsernameLabel->setText(QCoreApplication::translate("CancellationWindow", "Username", nullptr));
        PasswordLabel->setText(QCoreApplication::translate("CancellationWindow", "Password", nullptr));
        PasswordEdit->setText(QString());
        DeleteButton->setText(QCoreApplication::translate("CancellationWindow", "Delete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CancellationWindow: public Ui_CancellationWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANCELLATIONWINDOW_H
