/********************************************************************************
** Form generated from reading UI file 'server.ui'
**
** Created by: Qt User Interface Compiler version 5.9.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVER_H
#define UI_SERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Server
{
public:
    QLabel *stato;
    QPushButton *esci;

    void setupUi(QDialog *Server)
    {
        if (Server->objectName().isEmpty())
            Server->setObjectName(QStringLiteral("Server"));
        Server->resize(260, 223);
        stato = new QLabel(Server);
        stato->setObjectName(QStringLiteral("stato"));
        stato->setGeometry(QRect(10, 10, 231, 151));
        stato->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        stato->setTextInteractionFlags(Qt::TextBrowserInteraction);
        esci = new QPushButton(Server);
        esci->setObjectName(QStringLiteral("esci"));
        esci->setGeometry(QRect(90, 180, 81, 25));
        esci->setAutoDefault(false);

        retranslateUi(Server);

        QMetaObject::connectSlotsByName(Server);
    } // setupUi

    void retranslateUi(QDialog *Server)
    {
        Server->setWindowTitle(QApplication::translate("Server", "Dialog", Q_NULLPTR));
        stato->setText(QString());
        esci->setText(QApplication::translate("Server", "Esci", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Server: public Ui_Server {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVER_H
