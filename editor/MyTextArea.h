//
// Created by antonio on 04/05/20.
//

#ifndef EDITOR_MYTEXTAREA_H
#define EDITOR_MYTEXTAREA_H

#include <QtWidgets/QTextEdit>
#include <QKeyEvent>
#include "Symbol.h"
#include "FracPosition.h"
#include <QtWidgets>

#define DEBUG false

#define HEAD    0
#define BACK    1
#define MIDDLE  2


#define BATCH_CHAR_DELETION "deleteBatchChar"
#define SINGLE_CHAR_DELETION "delete1Char"
#define SINGLE_CHAR_ADDITION "add1Char"
#define BATCH_CHAR_ADDITION "addBatchChar"
#define CHANGE_CHAR_FORMAT "changeCharFormat"

#define KEY_IS_BACKSPACE(e) (e->key() == Qt::Key_Backspace)
#define KEY_IS_CUT(e) (e->modifiers() & Qt::ControlModifier && e->key() == Qt::Key_X)
#define KEY_IS_A_CHAR(e) (e->key() >= Qt::Key_Space && e->key() <= Qt::Key_ydiaeresis || e->key() == Qt::Key_Return)
#define KEY_IS_PASTE(e) (e->key() == Qt::Key_V && e->modifiers() & Qt::ControlModifier)
#define KEY_IS_COPY(e) (e->key() == Qt::Key_C && e->modifiers() & Qt::ControlModifier)
#define KEY_IS_BOLD(e) ((e->key() == Qt::Key_B) && (e->modifiers() & Qt::ControlModifier))
#define KEY_IS_ITALIC(e) (e->key() == Qt::Key_I && e->modifiers() & Qt::ControlModifier)
#define KEY_IS_UNDERLINE(e) (e->key() == Qt::Key_U && e->modifiers() & Qt::ControlModifier)
#define KEY_IS_SELECT_ALL(e) (e->key() == Qt::Key_A && e->modifiers() & Qt::ControlModifier)
#define KEY_IS_ARROW(e) (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down || e->key() == Qt::Key_Right || e->key() == Qt::Key_Left)
#define KEY_CTRL_IS_ON(e) (e->modifiers() & Qt::ControlModifier)

class MyTextArea : public QTextEdit {
Q_OBJECT
private:
    QString thisEditorIdentifier = "AAAA";
    int charCounter;
    QMap<FracPosition, Symbol> _symbols;
    int currentPosition = 0;
    int lastPosition = 0;
    bool selectionMode = false;
    int anchor;
    QClipboard *clipboard = QGuiApplication::clipboard();
    int oldPosition = 0;
    bool isBold = false;
    bool isStrike = false;
    bool isItalic = false;
    bool isUnderline = false;

public:
    MyTextArea();
    explicit MyTextArea(QWidget *parent);

    static const QMetaObject &getStaticMetaObject();

    bool isBold1() const;

    void setIsBold(bool isBold);

    bool isItalic1() const;

    void setIsItalic(bool isItalic);

    bool isUnderline1() const;

    void setIsUnderline(bool isUnderline);

    const QString &getThisEditorIdentifier() const;
    void insertChar(QChar unicode, int position, const QTextCharFormat& format);
    void insertBatchChar(QString text, int position, QVector<QTextCharFormat> formats);
    void deleteChar(int position);
    void deleteBatchChar(int begin, int end);
    void changeSelectionFormat(int begin, int end, QTextCharFormat format);

    void setThisEditorIdentifier(const QString &thisEditorIdentifier);
    void keyPressEvent(QKeyEvent *e) override;
    const QMap<FracPosition, Symbol> &getSymbols() const;
    virtual void inputMethodEvent(QInputMethodEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
    MyTextArea &operator=(const MyTextArea &other);
    void addSymbolToList(const Symbol& sym, QString color);
    void removeSymbolFromList(QString &symId, QString &fp, QString color);
    void changeSymbolFormat(QString &symId, QString &fp, QTextCharFormat format);
    int getEditorPosition(const FracPosition &fp);

    int getCurrentPosition() const;

    const Symbol &getSymbolFromPosition(int position);

    QVector<Symbol> getSymbolInRange(int end1, int end2);

public slots:

    virtual void insertFromMimeData(const QMimeData *source) override;

    void removeHighlight(const Symbol& sym);
    void updateStyle(const QTextCharFormat &f);


signals:
    void charInserted(QJsonObject message);
    void boldFormatActivate();
    void italicFormatActivate();
    void underlineFormatActivate();
    void charDeleted(QJsonObject message);
    void batchCharDelete(QJsonObject message);
    void batchCharInserted(QJsonArray message, QVector<QTextCharFormat> formats);
    void formatCharChanged(QJsonObject message);
};


#endif //EDITOR_MYTEXTAREA_H
