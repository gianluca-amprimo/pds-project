//
// Created by antonio on 04/05/20.
//

#ifndef EDITOR_MYTEXTEDITOR_H
#define EDITOR_MYTEXTEDITOR_H

#include <QtWidgets/QTextEdit>
#include <QKeyEvent>
#include "Symbol.h"
#include <QtWidgets>

#define HEAD    0
#define BACK    1
#define MIDDLE  2


class MyTextEditor : public QTextEdit{
    Q_OBJECT

public:

    explicit MyTextEditor(QWidget *parent);
    const QString &getThisEditorIdentifier() const;
    void deleteSymbol();
    void deleteSelection();
    void insertSymbol(QChar character, int position);
    void setThisEditorIdentifier(const QString &thisEditorIdentifier);
    void keyPressEvent(QKeyEvent *e) override;
    void generateFile();
    void loadFromFile();
    virtual void inputMethodEvent(QInputMethodEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *e);


public slots:
    virtual void insertFromMimeData(const QMimeData *source);


private:
    QString thisEditorIdentifier = "AAAA";
    int charCounter;
    QVector<Symbol> _symbols;
    int currentPosition = 0;
    int oldPosition = 0;
    int lastPosition = 0;
    bool selectionMode = false;
    bool pasting = false;
    int anchor;

};


#endif //EDITOR_MYTEXTEDITOR_H
