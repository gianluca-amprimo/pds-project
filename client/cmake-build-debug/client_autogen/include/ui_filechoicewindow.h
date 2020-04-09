/********************************************************************************
** Form generated from reading UI file 'filechoicewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILECHOICEWINDOW_H
#define UI_FILECHOICEWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FileChoiceWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *WelcomeLabel;
    QPushButton *NewButton;
    QHBoxLayout *horizontalLayout;
    QComboBox *OpenMenu;
    QPushButton *OpenButton;
    QPushButton *SettingsButton;

    void setupUi(QDialog *FileChoiceWindow)
    {
        if (FileChoiceWindow->objectName().isEmpty())
            FileChoiceWindow->setObjectName(QString::fromUtf8("FileChoiceWindow"));
        FileChoiceWindow->resize(236, 150);
        verticalLayout = new QVBoxLayout(FileChoiceWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        WelcomeLabel = new QLabel(FileChoiceWindow);
        WelcomeLabel->setObjectName(QString::fromUtf8("WelcomeLabel"));
        QFont font;
        font.setPointSize(24);
        WelcomeLabel->setFont(font);
        WelcomeLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(WelcomeLabel);

        NewButton = new QPushButton(FileChoiceWindow);
        NewButton->setObjectName(QString::fromUtf8("NewButton"));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("document-new");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        NewButton->setIcon(icon);

        verticalLayout->addWidget(NewButton);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        OpenMenu = new QComboBox(FileChoiceWindow);
        OpenMenu->setObjectName(QString::fromUtf8("OpenMenu"));
        OpenMenu->setAutoFillBackground(true);
        OpenMenu->setEditable(true);
        OpenMenu->setInsertPolicy(QComboBox::NoInsert);

        horizontalLayout->addWidget(OpenMenu);

        OpenButton = new QPushButton(FileChoiceWindow);
        OpenButton->setObjectName(QString::fromUtf8("OpenButton"));
        OpenButton->setMaximumSize(QSize(80, 16777215));
        QIcon icon1;
        iconThemeName = QString::fromUtf8("document-open");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        OpenButton->setIcon(icon1);

        horizontalLayout->addWidget(OpenButton);


        verticalLayout->addLayout(horizontalLayout);

        SettingsButton = new QPushButton(FileChoiceWindow);
        SettingsButton->setObjectName(QString::fromUtf8("SettingsButton"));
        QIcon icon2;
        iconThemeName = QString::fromUtf8("emblem-system");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon2 = QIcon::fromTheme(iconThemeName);
        } else {
            icon2.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        SettingsButton->setIcon(icon2);

        verticalLayout->addWidget(SettingsButton);


        retranslateUi(FileChoiceWindow);

        NewButton->setDefault(true);


        QMetaObject::connectSlotsByName(FileChoiceWindow);
    } // setupUi

    void retranslateUi(QDialog *FileChoiceWindow)
    {
        FileChoiceWindow->setWindowTitle(QCoreApplication::translate("FileChoiceWindow", "File choice", nullptr));
        WelcomeLabel->setText(QCoreApplication::translate("FileChoiceWindow", "Welcome back!", nullptr));
        NewButton->setText(QCoreApplication::translate("FileChoiceWindow", "New file", nullptr));
        OpenButton->setText(QCoreApplication::translate("FileChoiceWindow", "Open", nullptr));
        SettingsButton->setText(QCoreApplication::translate("FileChoiceWindow", "Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FileChoiceWindow: public Ui_FileChoiceWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILECHOICEWINDOW_H
