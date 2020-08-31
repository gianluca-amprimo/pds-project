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


class MyTextArea : public QTextEdit {
Q_OBJECT

public:
    MyTextArea();

    explicit MyTextArea(QWidget *parent);

    const QString &getThisEditorIdentifier() const;

    void deleteSymbol();

    void deleteSelection();

    void insertSymbol(QChar character, int position);

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

    void symbolReady(Symbol &rsymbol);

    void symbolDeleted(QByteArray symId);


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
};


#endif //EDITOR_MYTEXTAREA_H
