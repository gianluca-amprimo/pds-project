#ifndef PIDIESSE_MAINEDITOR_H
#define PIDIESSE_MAINEDITOR_H

#include <QMainWindow>
#include <QComboBox>
#include <QtWidgets/QFontComboBox>
#include <QtNetwork/QTcpSocket>
#include "ui_MainEditor.h"
#include "Symbol.h"
#include "MyTextArea.h"
#include <QtPrintSupport/QPrinter>

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

    void receiveSymbol(QJsonValueRef content, QJsonValueRef username);

    void receiveBatchSymbol(QJsonArray content);

    void receiveDeletion(QJsonValueRef id, QJsonValueRef position, QJsonValueRef username);

    void receiveBatchDeletion(QJsonValueRef idsAndPositions, QJsonValueRef user);
    void receiveFormatChanged(QJsonValueRef idsAndPositions, QJsonValueRef user);
    const QString &getFilename() const;

    void colors(QString username, QString color);

private:
    QString thisEditorIdentifier;
    Ui::MainEditor *ui;
    QString filename;
    QString username;
    QTcpSocket *tcpSocket;

    QFontComboBox *fontSelector;
    QComboBox *sizeSelector;
    MyTextArea *textArea;
    QListWidget *onlineUsers;

    QAction *pdf, *share;

    QMap<QString, QString> userMap;         // user -> color

    int position;

    void initUI(QDataStream *contentStream);

    void setupActions();

private slots:
    void Bold();
    void Italic();
    void Underline();

    void selectFont();
    void selectSize();

    void updateCharFormat();

    void save();
    void closeEvent(QCloseEvent *event) override;

    void sendCharInserted(QJsonObject message);

    void sendCharDeleted(QJsonObject message);
    void sendBatchCharDeleted(QJsonObject message);

    void sendBatchCharInserted(QJsonArray message, QVector<QTextCharFormat> formats);
    void sendCharFormatChanged(QJsonObject message);

    void shareLink();
    void exportAsPDF();
    void updateStyleButton(const QTextCharFormat &f);

};



#endif //PIDIESSE_MAINEDITOR_H
