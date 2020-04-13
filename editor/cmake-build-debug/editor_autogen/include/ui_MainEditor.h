/********************************************************************************
** Form generated from reading UI file 'MainEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.12.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINEDITOR_H
#define UI_MAINEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainEditor
{
public:
    QAction *actionboh;
    QAction *saveAs;
    QAction *open;
    QAction *cut;
    QAction *copy;
    QAction *paste;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTextEdit *textArea;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;

    void setupUi(QMainWindow *MainEditor)
    {
        if (MainEditor->objectName().isEmpty())
            MainEditor->setObjectName(QString::fromUtf8("MainEditor"));
        MainEditor->resize(839, 744);
        MainEditor->setAutoFillBackground(false);
        actionboh = new QAction(MainEditor);
        actionboh->setObjectName(QString::fromUtf8("actionboh"));
        saveAs = new QAction(MainEditor);
        saveAs->setObjectName(QString::fromUtf8("saveAs"));
        open = new QAction(MainEditor);
        open->setObjectName(QString::fromUtf8("open"));
        cut = new QAction(MainEditor);
        cut->setObjectName(QString::fromUtf8("cut"));
        copy = new QAction(MainEditor);
        copy->setObjectName(QString::fromUtf8("copy"));
        paste = new QAction(MainEditor);
        paste->setObjectName(QString::fromUtf8("paste"));
        centralwidget = new QWidget(MainEditor);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setLayoutDirection(Qt::RightToLeft);
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textArea = new QTextEdit(centralwidget);
        textArea->setObjectName(QString::fromUtf8("textArea"));

        gridLayout->addWidget(textArea, 0, 0, 1, 1);

        MainEditor->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MainEditor);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 839, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        MainEditor->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuFile->addAction(open);
        menuFile->addAction(saveAs);
        menuEdit->addAction(cut);
        menuEdit->addAction(copy);
        menuEdit->addAction(paste);

        retranslateUi(MainEditor);

        QMetaObject::connectSlotsByName(MainEditor);
    } // setupUi

    void retranslateUi(QMainWindow *MainEditor)
    {
        MainEditor->setWindowTitle(QApplication::translate("MainEditor", "PiDiEsse", nullptr));
        actionboh->setText(QApplication::translate("MainEditor", "boh", nullptr));
        saveAs->setText(QApplication::translate("MainEditor", "Save As", nullptr));
#ifndef QT_NO_SHORTCUT
        saveAs->setShortcut(QApplication::translate("MainEditor", "Ctrl+Shift+S", nullptr));
#endif // QT_NO_SHORTCUT
        open->setText(QApplication::translate("MainEditor", "Open", nullptr));
#ifndef QT_NO_SHORTCUT
        open->setShortcut(QApplication::translate("MainEditor", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        cut->setText(QApplication::translate("MainEditor", "Copy", nullptr));
#ifndef QT_NO_SHORTCUT
        cut->setShortcut(QApplication::translate("MainEditor", "Ctrl+X", nullptr));
#endif // QT_NO_SHORTCUT
        copy->setText(QApplication::translate("MainEditor", "Cut", nullptr));
#ifndef QT_NO_SHORTCUT
        copy->setShortcut(QApplication::translate("MainEditor", "Ctrl+C", nullptr));
#endif // QT_NO_SHORTCUT
        paste->setText(QApplication::translate("MainEditor", "Paste", nullptr));
#ifndef QT_NO_SHORTCUT
        paste->setShortcut(QApplication::translate("MainEditor", "Ctrl+V", nullptr));
#endif // QT_NO_SHORTCUT
        menuFile->setTitle(QApplication::translate("MainEditor", "File", nullptr));
        menuEdit->setTitle(QApplication::translate("MainEditor", "Edit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainEditor: public Ui_MainEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINEDITOR_H
