/********************************************************************************
** Form generated from reading UI file 'MainEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINEDITOR_H
#define UI_MAINEDITOR_H

#include "../../../MyTextEditor.h"
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainEditor
{
public:
    QAction *saveAs;
    QAction *openDoc;
    QAction *cut;
    QAction *copy;
    QAction *paste;
    QAction *bold;
    QAction *italic;
    QAction *underline;
    QAction *save;
    QAction *newDoc;
    QAction *activeUsers;
    QAction *alignLeft;
    QAction *alignRight;
    QAction *alignJustified;
    QAction *alignCenter;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    MyTextEditor *textArea;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuStyling;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainEditor)
    {
        if (MainEditor->objectName().isEmpty())
            MainEditor->setObjectName(QString::fromUtf8("MainEditor"));
        MainEditor->resize(839, 744);
        MainEditor->setAutoFillBackground(false);
        saveAs = new QAction(MainEditor);
        saveAs->setObjectName(QString::fromUtf8("saveAs"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/file/themes/material/save_as.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveAs->setIcon(icon);
        openDoc = new QAction(MainEditor);
        openDoc->setObjectName(QString::fromUtf8("openDoc"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/file/themes/material/open_doc.png"), QSize(), QIcon::Normal, QIcon::Off);
        openDoc->setIcon(icon1);
        cut = new QAction(MainEditor);
        cut->setObjectName(QString::fromUtf8("cut"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/text_edit/themes/material/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
        cut->setIcon(icon2);
        copy = new QAction(MainEditor);
        copy->setObjectName(QString::fromUtf8("copy"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/text_edit/themes/material/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
        copy->setIcon(icon3);
        paste = new QAction(MainEditor);
        paste->setObjectName(QString::fromUtf8("paste"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/text_edit/themes/material/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
        paste->setIcon(icon4);
        bold = new QAction(MainEditor);
        bold->setObjectName(QString::fromUtf8("bold"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/text_styling/themes/material/bold.png"), QSize(), QIcon::Normal, QIcon::Off);
        bold->setIcon(icon5);
        italic = new QAction(MainEditor);
        italic->setObjectName(QString::fromUtf8("italic"));
        QIcon icon6;
        QString iconThemeName = QString::fromUtf8("Material");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon6 = QIcon::fromTheme(iconThemeName);
        } else {
            icon6.addFile(QString::fromUtf8(":/text_styling/themes/material/italic.png"), QSize(), QIcon::Normal, QIcon::Off);
        }
        italic->setIcon(icon6);
        underline = new QAction(MainEditor);
        underline->setObjectName(QString::fromUtf8("underline"));
        QIcon icon7;
        iconThemeName = QString::fromUtf8("Material");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon7 = QIcon::fromTheme(iconThemeName);
        } else {
            icon7.addFile(QString::fromUtf8(":/text_styling/themes/material/underline.png"), QSize(), QIcon::Normal, QIcon::Off);
        }
        underline->setIcon(icon7);
        save = new QAction(MainEditor);
        save->setObjectName(QString::fromUtf8("save"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/file/themes/material/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        save->setIcon(icon8);
        newDoc = new QAction(MainEditor);
        newDoc->setObjectName(QString::fromUtf8("newDoc"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/file/themes/material/new_doc.png"), QSize(), QIcon::Normal, QIcon::Off);
        newDoc->setIcon(icon9);
        activeUsers = new QAction(MainEditor);
        activeUsers->setObjectName(QString::fromUtf8("activeUsers"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/file/themes/material/user.png"), QSize(), QIcon::Normal, QIcon::Off);
        activeUsers->setIcon(icon10);
        alignLeft = new QAction(MainEditor);
        alignLeft->setObjectName(QString::fromUtf8("alignLeft"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/text_align/themes/material/align_left.png"), QSize(), QIcon::Normal, QIcon::Off);
        alignLeft->setIcon(icon11);
        alignRight = new QAction(MainEditor);
        alignRight->setObjectName(QString::fromUtf8("alignRight"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/text_align/themes/material/align_right.png"), QSize(), QIcon::Normal, QIcon::Off);
        alignRight->setIcon(icon12);
        alignJustified = new QAction(MainEditor);
        alignJustified->setObjectName(QString::fromUtf8("alignJustified"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/text_align/themes/material/align_justified.png"), QSize(), QIcon::Normal, QIcon::Off);
        alignJustified->setIcon(icon13);
        alignCenter = new QAction(MainEditor);
        alignCenter->setObjectName(QString::fromUtf8("alignCenter"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/text_align/themes/material/align_center.png"), QSize(), QIcon::Normal, QIcon::Off);
        alignCenter->setIcon(icon14);
        centralwidget = new QWidget(MainEditor);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setLayoutDirection(Qt::RightToLeft);
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textArea = new MyTextEditor(centralwidget);
        textArea->setObjectName(QString::fromUtf8("textArea"));

        gridLayout->addWidget(textArea, 1, 0, 1, 1);

        MainEditor->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MainEditor);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 839, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuStyling = new QMenu(menuBar);
        menuStyling->setObjectName(QString::fromUtf8("menuStyling"));
        MainEditor->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainEditor);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainEditor->setStatusBar(statusBar);
        toolBar = new QToolBar(MainEditor);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        MainEditor->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuStyling->menuAction());
        menuFile->addAction(newDoc);
        menuFile->addAction(openDoc);
        menuFile->addAction(save);
        menuFile->addAction(saveAs);
        menuEdit->addAction(cut);
        menuEdit->addAction(copy);
        menuEdit->addAction(paste);
        menuStyling->addAction(bold);
        menuStyling->addAction(italic);
        menuStyling->addAction(underline);
        toolBar->addAction(activeUsers);
        toolBar->addSeparator();
        toolBar->addAction(bold);
        toolBar->addAction(italic);
        toolBar->addAction(underline);
        toolBar->addSeparator();
        toolBar->addAction(alignLeft);
        toolBar->addAction(alignCenter);
        toolBar->addAction(alignRight);
        toolBar->addAction(alignJustified);
        toolBar->addSeparator();

        retranslateUi(MainEditor);

        QMetaObject::connectSlotsByName(MainEditor);
    } // setupUi

    void retranslateUi(QMainWindow *MainEditor)
    {
        MainEditor->setWindowTitle(QApplication::translate("MainEditor", "PiDiEsse", nullptr));
        saveAs->setText(QApplication::translate("MainEditor", "Save As", nullptr));
#ifndef QT_NO_TOOLTIP
        saveAs->setToolTip(QApplication::translate("MainEditor", "Save as a new file", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        saveAs->setShortcut(QApplication::translate("MainEditor", "Ctrl+Shift+S", nullptr));
#endif // QT_NO_SHORTCUT
        openDoc->setText(QApplication::translate("MainEditor", "Open", nullptr));
#ifndef QT_NO_TOOLTIP
        openDoc->setToolTip(QApplication::translate("MainEditor", "Open a document", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        openDoc->setShortcut(QApplication::translate("MainEditor", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        cut->setText(QApplication::translate("MainEditor", "Cut", nullptr));
#ifndef QT_NO_TOOLTIP
        cut->setToolTip(QApplication::translate("MainEditor", "Cut highlighted content", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        cut->setShortcut(QApplication::translate("MainEditor", "Ctrl+X", nullptr));
#endif // QT_NO_SHORTCUT
        copy->setText(QApplication::translate("MainEditor", "Copy", nullptr));
#ifndef QT_NO_TOOLTIP
        copy->setToolTip(QApplication::translate("MainEditor", "Copy to clipboard", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        copy->setShortcut(QApplication::translate("MainEditor", "Ctrl+C", nullptr));
#endif // QT_NO_SHORTCUT
        paste->setText(QApplication::translate("MainEditor", "Paste", nullptr));
#ifndef QT_NO_TOOLTIP
        paste->setToolTip(QApplication::translate("MainEditor", "Paste from clipboard", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        paste->setShortcut(QApplication::translate("MainEditor", "Ctrl+V", nullptr));
#endif // QT_NO_SHORTCUT
        bold->setText(QApplication::translate("MainEditor", "Bold", nullptr));
#ifndef QT_NO_TOOLTIP
        bold->setToolTip(QApplication::translate("MainEditor", "<html><head/><body><p><span style=\" font-weight:600;\">Bold</span></p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        bold->setShortcut(QApplication::translate("MainEditor", "Ctrl+B", nullptr));
#endif // QT_NO_SHORTCUT
        italic->setText(QApplication::translate("MainEditor", "Italic", nullptr));
#ifndef QT_NO_TOOLTIP
        italic->setToolTip(QApplication::translate("MainEditor", "<html><head/><body><p><span style=\" font-style:italic;\">Italic</span></p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        italic->setShortcut(QApplication::translate("MainEditor", "Ctrl+I", nullptr));
#endif // QT_NO_SHORTCUT
        underline->setText(QApplication::translate("MainEditor", "Underline", nullptr));
#ifndef QT_NO_TOOLTIP
        underline->setToolTip(QApplication::translate("MainEditor", "<html><head/><body><p><span style=\" text-decoration: underline;\">Underline</span></p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        underline->setShortcut(QApplication::translate("MainEditor", "Ctrl+U", nullptr));
#endif // QT_NO_SHORTCUT
        save->setText(QApplication::translate("MainEditor", "Save", nullptr));
#ifndef QT_NO_TOOLTIP
        save->setToolTip(QApplication::translate("MainEditor", "Save the current file", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        save->setShortcut(QApplication::translate("MainEditor", "Ctrl+S", nullptr));
#endif // QT_NO_SHORTCUT
        newDoc->setText(QApplication::translate("MainEditor", "New", nullptr));
#ifndef QT_NO_TOOLTIP
        newDoc->setToolTip(QApplication::translate("MainEditor", "Create a new document", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        newDoc->setShortcut(QApplication::translate("MainEditor", "Ctrl+N", nullptr));
#endif // QT_NO_SHORTCUT
        activeUsers->setText(QApplication::translate("MainEditor", "Active Users", nullptr));
#ifndef QT_NO_TOOLTIP
        activeUsers->setToolTip(QApplication::translate("MainEditor", "Show active users information", nullptr));
#endif // QT_NO_TOOLTIP
        alignLeft->setText(QApplication::translate("MainEditor", "Align Left", nullptr));
#ifndef QT_NO_TOOLTIP
        alignLeft->setToolTip(QApplication::translate("MainEditor", "Align selected paragraphs to the left", nullptr));
#endif // QT_NO_TOOLTIP
        alignRight->setText(QApplication::translate("MainEditor", "Align Right", nullptr));
#ifndef QT_NO_TOOLTIP
        alignRight->setToolTip(QApplication::translate("MainEditor", "Align selected paragraphs to the right", nullptr));
#endif // QT_NO_TOOLTIP
        alignJustified->setText(QApplication::translate("MainEditor", "Align Justified", nullptr));
#ifndef QT_NO_TOOLTIP
        alignJustified->setToolTip(QApplication::translate("MainEditor", "Justify selected paragraphs", nullptr));
#endif // QT_NO_TOOLTIP
        alignCenter->setText(QApplication::translate("MainEditor", "Align Center", nullptr));
#ifndef QT_NO_TOOLTIP
        alignCenter->setToolTip(QApplication::translate("MainEditor", "Align selected paragraphs to the center", nullptr));
#endif // QT_NO_TOOLTIP
        menuFile->setTitle(QApplication::translate("MainEditor", "File", nullptr));
        menuEdit->setTitle(QApplication::translate("MainEditor", "Edit", nullptr));
        menuStyling->setTitle(QApplication::translate("MainEditor", "Format", nullptr));
        toolBar->setWindowTitle(QApplication::translate("MainEditor", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainEditor: public Ui_MainEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINEDITOR_H
