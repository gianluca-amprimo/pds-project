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
    QPushButton *RefreshButton;
    QPushButton *OpenButton;
    QPushButton *SettingsButton;
    QPushButton *LogoutButton;

    void setupUi(QDialog *FileChoiceWindow)
    {
        if (FileChoiceWindow->objectName().isEmpty())
            FileChoiceWindow->setObjectName(QStringLiteral("FileChoiceWindow"));
        FileChoiceWindow->resize(267, 269);
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

        RefreshButton = new QPushButton(FileChoiceWindow);
        RefreshButton->setObjectName(QStringLiteral("RefreshButton"));
        RefreshButton->setMaximumSize(QSize(24, 16777215));
        QIcon icon1(QIcon::fromTheme(QStringLiteral("view-refresh")));
        RefreshButton->setIcon(icon1);

        horizontalLayout->addWidget(RefreshButton);

        OpenButton = new QPushButton(FileChoiceWindow);
        OpenButton->setObjectName(QStringLiteral("OpenButton"));
        OpenButton->setMaximumSize(QSize(70, 16777215));
        QIcon icon2;
        iconThemeName = QStringLiteral("document-open");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon2 = QIcon::fromTheme(iconThemeName);
        } else {
            icon2.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        OpenButton->setIcon(icon2);

        horizontalLayout->addWidget(OpenButton);


        verticalLayout->addLayout(horizontalLayout);

        SettingsButton = new QPushButton(FileChoiceWindow);
        SettingsButton->setObjectName(QStringLiteral("SettingsButton"));
        QIcon icon3;
        iconThemeName = QStringLiteral("emblem-system");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon3 = QIcon::fromTheme(iconThemeName);
        } else {
            icon3.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        SettingsButton->setIcon(icon3);

        verticalLayout->addWidget(SettingsButton);

        LogoutButton = new QPushButton(FileChoiceWindow);
        LogoutButton->setObjectName(QStringLiteral("LogoutButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(LogoutButton->sizePolicy().hasHeightForWidth());
        LogoutButton->setSizePolicy(sizePolicy1);
        LogoutButton->setMaximumSize(QSize(100, 25));
        QIcon icon4;
        iconThemeName = QStringLiteral("gnome-logout");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon4 = QIcon::fromTheme(iconThemeName);
        } else {
            icon4.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        LogoutButton->setIcon(icon4);

        verticalLayout->addWidget(LogoutButton, 0, Qt::AlignRight);


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
#ifndef QT_NO_TOOLTIP
        RefreshButton->setToolTip(QApplication::translate("FileChoiceWindow", "<html><head/><body><p>Refresh file list</p></body></html>", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        RefreshButton->setText(QString());
        OpenButton->setText(QApplication::translate("FileChoiceWindow", "Open", Q_NULLPTR));
        SettingsButton->setText(QApplication::translate("FileChoiceWindow", "Settings", Q_NULLPTR));
        LogoutButton->setText(QApplication::translate("FileChoiceWindow", "Logout", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class FileChoiceWindow: public Ui_FileChoiceWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILECHOICEWINDOW_H
