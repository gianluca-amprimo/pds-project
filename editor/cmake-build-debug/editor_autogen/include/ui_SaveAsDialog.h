/********************************************************************************
** Form generated from reading UI file 'SaveAsDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEASDIALOG_H
#define UI_SAVEASDIALOG_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SaveAsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SaveAsDialog)
    {
        if (SaveAsDialog->objectName().isEmpty())
            SaveAsDialog->setObjectName(QString::fromUtf8("SaveAsDialog"));
        SaveAsDialog->resize(357, 76);
        SaveAsDialog->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        verticalLayout = new QVBoxLayout(SaveAsDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(SaveAsDialog);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(SaveAsDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        horizontalLayout->addWidget(lineEdit);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(SaveAsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(SaveAsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SaveAsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SaveAsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(SaveAsDialog);
    } // setupUi

    void retranslateUi(QDialog *SaveAsDialog)
    {
        SaveAsDialog->setWindowTitle(QCoreApplication::translate("SaveAsDialog", "Save as ...", nullptr));
        label->setText(QCoreApplication::translate("SaveAsDialog", "File name", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SaveAsDialog: public Ui_SaveAsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEASDIALOG_H
