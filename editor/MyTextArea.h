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

#define HEAD    0
#define BACK    1
#define MIDDLE  2


#define BATCH_CHAR_DELETION "deleteBatchChar"
#define SINGLE_CHAR_DELETION "delete1Char"
#define SINGLE_CHAR_ADDITION "add1Char"
#define BATCH_CHAR_ADDITION "addBatchChar"

#define KEY_IS_BACKSPACE (e->key() == Qt::Key_Backspace)
#define KEY_IS_CUT (e->modifiers() & Qt::ControlModifier && e->key() == Qt::Key_X)
#define KEY_IS_A_CHAR (e->key() >= Qt::Key_Space && e->key() <= Qt::Key_ydiaeresis || e->key() == Qt::Key_Return)
#define KEY_IS_PASTE (e->key() == Qt::Key_V && e->modifiers() & Qt::ControlModifier)
#define KEY_IS_COPY (e->key() == Qt::Key_C && e->modifiers() & Qt::ControlModifier)
#define KEY_IS_SELECT_ALL (e->key() == Qt::Key_A && e->modifiers() & Qt::ControlModifier)
#define KEY_IS_ARROW (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down || e->key() == Qt::Key_Right || e->key() == Qt::Key_Left)
#define KEY_CTRL_IS_ON (e->modifiers() & Qt::ControlModifier)

class MyTextArea : public QTextEdit {
Q_OBJECT

public:
    MyTextArea();
    explicit MyTextArea(QWidget *parent);
    const QString &getThisEditorIdentifier() const;
    void deleteSymbol();
    void deleteSelection();
    void insertChar(QChar unicode, int position, QTextCharFormat format);
    void insertBatchChar(QString text, int position, QVector<QTextCharFormat> formats);
    void deleteChar(int position);
    void deleteBatchChar(int begin, int end);

    void setThisEditorIdentifier(const QString &thisEditorIdentifier);
    void keyPressEvent(QKeyEvent *e) override;
    const QMap<FracPosition, Symbol> &getSymbols() const;
    virtual void inputMethodEvent(QInputMethodEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;
    MyTextArea &operator=(const MyTextArea &other);
    void addSymbolToList(Symbol sym);
    void removeSymbolFromList(QString &symId, QString &fp);
    int getEditorPosition(const FracPosition &fp);
    const Symbol &getSymbolFromPosition(int position);
    QVector<Symbol> getSymbolInRange(int end1, int end2);


public slots:

    virtual void insertFromMimeData(const QMimeData *source) override;

signals:

    void charInserted(QJsonObject message);
    void charDeleted(QJsonObject message);
    void batchCharDelete(QJsonObject message);
    void batchCharInserted(QJsonArray message, QVector<QTextCharFormat> formats);


private:
    QString thisEditorIdentifier = "AAAA";
    int charCounter;
    QMap<FracPosition, Symbol> _symbols;
    int currentPosition = 0;
    int oldPosition = 0;
    int lastPosition = 0;
    bool selectionMode = false;
    bool pasting = false;
    int anchor;
    QClipboard *clipboard = QGuiApplication::clipboard();
};


#endif //EDITOR_MYTEXTAREA_H
