/********************************************************************************
** Form generated from reading UI file 'filechoicewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILECHOICEWINDOW_H
#define UI_FILECHOICEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FileChoiceWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *ProfilePicture;
    QLabel *WelcomeLabel;
    QPushButton *NewButton;
    QHBoxLayout *horizontalLayout;
    QComboBox *OpenMenu;
    QPushButton *OpenButton;
    QPushButton *SettingsButton;

    void setupUi(QDialog *FileChoiceWindow)
    {
        if (FileChoiceWindow->objectName().isEmpty())
            FileChoiceWindow->setObjectName(QStringLiteral("FileChoiceWindow"));
        FileChoiceWindow->resize(236, 269);
        verticalLayout = new QVBoxLayout(FileChoiceWindow);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        ProfilePicture = new QLabel(FileChoiceWindow);
        ProfilePicture->setObjectName(QStringLiteral("ProfilePicture"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ProfilePicture->sizePolicy().hasHeightForWidth());
        ProfilePicture->setSizePolicy(sizePolicy);
        ProfilePicture->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(ProfilePicture);

        WelcomeLabel = new QLabel(FileChoiceWindow);
        WelcomeLabel->setObjectName(QStringLiteral("WelcomeLabel"));
        QFont font;
        font.setPointSize(24);
        WelcomeLabel->setFont(font);
        WelcomeLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(WelcomeLabel);

        NewButton = new QPushButton(FileChoiceWindow);
        NewButton->setObjectName(QStringLiteral("NewButton"));
        QIcon icon;
        QString iconThemeName = QStringLiteral("document-new");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        NewButton->setIcon(icon);

        verticalLayout->addWidget(NewButton);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        OpenMenu = new QComboBox(FileChoiceWindow);
        OpenMenu->setObjectName(QStringLiteral("OpenMenu"));
        OpenMenu->setAutoFillBackground(true);
        OpenMenu->setEditable(true);
        OpenMenu->setInsertPolicy(QComboBox::NoInsert);

        horizontalLayout->addWidget(OpenMenu);

        OpenButton = new QPushButton(FileChoiceWindow);
        OpenButton->setObjectName(QStringLiteral("OpenButton"));
        OpenButton->setMaximumSize(QSize(80, 16777215));
        QIcon icon1;
        iconThemeName = QStringLiteral("document-open");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        OpenButton->setIcon(icon1);

        horizontalLayout->addWidget(OpenButton);


        verticalLayout->addLayout(horizontalLayout);

        SettingsButton = new QPushButton(FileChoiceWindow);
        SettingsButton->setObjectName(QStringLiteral("SettingsButton"));
        QIcon icon2;
        iconThemeName = QStringLiteral("emblem-system");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon2 = QIcon::fromTheme(iconThemeName);
        } else {
            icon2.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        SettingsButton->setIcon(icon2);

        verticalLayout->addWidget(SettingsButton);


        retranslateUi(FileChoiceWindow);

        NewButton->setDefault(true);


        QMetaObject::connectSlotsByName(FileChoiceWindow);
    } // setupUi

    void retranslateUi(QDialog *FileChoiceWindow)
    {
        FileChoiceWindow->setWindowTitle(QApplication::translate("FileChoiceWindow", "File choice", Q_NULLPTR));
        ProfilePicture->setText(QString());
        WelcomeLabel->setText(QApplication::translate("FileChoiceWindow", "Welcome back!", Q_NULLPTR));
        NewButton->setText(QApplication::translate("FileChoiceWindow", "New file", Q_NULLPTR));
        OpenButton->setText(QApplication::translate("FileChoiceWindow", "Open", Q_NULLPTR));
        SettingsButton->setText(QApplication::translate("FileChoiceWindow", "Settings", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FileChoiceWindow: public Ui_FileChoiceWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILECHOICEWINDOW_H
