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
    const std::wstring &getThisEditorIdentifier() const;
    void deleteSymbol();
    void insertSymbol(wchar_t character);
    void setThisEditorIdentifier(const std::wstring &thisEditorIdentifier);
    void keyPressEvent(QKeyEvent *e) override;
    virtual void inputMethodEvent(QInputMethodEvent *event);

private:
    std::wstring thisEditorIdentifier = L"AAAA";
    int charCounter;
    std::vector<Symbol> _symbols;
    int currentPosition;
    int oldPosition;

};


#endif //EDITOR_MYTEXTEDITOR_H
