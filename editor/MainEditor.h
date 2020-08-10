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
    explicit MainEditor(QWidget *parent = nullptr, std::wstring editorIdentifier = L"AAAA", QString filename = "", QTcpSocket *tcpSocket = nullptr, QDataStream *contentSteam = nullptr);
    ~MainEditor() override;
    Ui::MainEditor *getUi();

private:
    std::wstring thisEditorIdentifier;
    Ui::MainEditor *ui;
    SaveAsDialog *saveAsDialog;
    QString filename;
    QTcpSocket *tcpSocket;

    QFontComboBox *fontSelector;
    QComboBox *sizeSelector;
    MyTextArea *textArea;
    int position;
    void initUI(QDataStream *mta);
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
    void closeEvent(QCloseEvent *event);
    void sendSymbol(QByteArray serializedSym);
};


#endif //PIDIESSE_MAINEDITOR_H
