#ifndef PIDIESSE_MAINEDITOR_H
#define PIDIESSE_MAINEDITOR_H

#include <QMainWindow>
#include <QComboBox>
#include <QtWidgets/QFontComboBox>
#include <QtNetwork/QTcpSocket>
#include "ui_MainEditor.h"
#include "ui_SaveAsDialog.h"
#include "SaveAsDialog.h"
#include "Symbol.h"
#include "MyTextArea.h"

#define HEAD    0
#define BACK    1
#define MIDDLE  2

QT_BEGIN_NAMESPACE
namespace Ui { class MainEditor; }
QT_END_NAMESPACE

class MainEditor : public QMainWindow {
Q_OBJECT
public:
    explicit MainEditor(QWidget *parent = nullptr, QString editorIdentifier = "", QString filename = "",
                        QTcpSocket *tcpSocket = nullptr, QDataStream *contentSteam = nullptr, QString username = "");

    ~MainEditor() override;

    Ui::MainEditor *getUi();

    void receiveSymbol(QJsonValueRef content);
    void receiveDeletion(QJsonValueRef id, QJsonValueRef position);

    void colors(QString username, QString color, QString postion);

private:
    QString thisEditorIdentifier;
    Ui::MainEditor *ui;
    SaveAsDialog *saveAsDialog;
    QString filename;
    QString username;
    QTcpSocket *tcpSocket;

    QFontComboBox *fontSelector;
    QComboBox *sizeSelector;
    MyTextArea *textArea;
    int position;

    QTimer* timer;

    void initUI(QDataStream *contentStream);

    void setupActions();

private slots:
    void Bold();
    void Italic();
    void Underline();

    void selectFont(const QString &font);
    void selectSize(const QString &size);

    void alignCenter();
    void alignLeft();
    void alignRight();
    void alignJustify();

    void updateCharFormat();

    void save();
    void closeEvent(QCloseEvent *event) override;

    void sendSymbol(Symbol &symbol);
    void sendDeletion(QByteArray serializedSymId);

    void sendPosition();
};


#endif //PIDIESSE_MAINEDITOR_H
