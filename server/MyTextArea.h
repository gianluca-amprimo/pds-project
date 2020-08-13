//
// Created by antonio on 04/05/20.
//

#ifndef EDITOR_MYTEXTAREA_H
#define EDITOR_MYTEXTAREA_H

#include <QtWidgets/QTextEdit>
#include <QKeyEvent>
#include "Symbol.h"
#include <QtWidgets>

#define HEAD    0
#define BACK    1
#define MIDDLE  2


class MyTextArea : public QTextEdit{
    Q_OBJECT

public:
    MyTextArea();
    explicit MyTextArea(QWidget *parent);
    const QString &getThisEditorIdentifier() const;
    void deleteSymbol();
    QVector<Symbol> get_symbols();
    void deleteSelection();
    void insertSymbol(QChar character, int position);
    void setThisEditorIdentifier(const QString &thisEditorIdentifier);
    void keyPressEvent(QKeyEvent *e) override;
    void generateFile();
    void loadFromFile();
    virtual void inputMethodEvent(QInputMethodEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    MyTextArea& operator=(const MyTextArea& other);


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

public:
    QDataStream& serialize(QDataStream& out) const;

    QDataStream& deserialize(QDataStream& in);

    friend QDataStream& operator<<(QDataStream& out, MyTextArea const& mta) {
        return mta.serialize(out);
    }

    friend QDataStream& operator>>(QDataStream& in, MyTextArea& mta) {
        return mta.deserialize(in);
    }

    friend std::ostream& operator<<(std::ostream& out, MyTextArea& mta) {
        // print something from v to str, e.g: Str << v.getX();
        for(Symbol sym : mta._symbols){
            out << sym.getCharacter().toLatin1();
        }
        return out;
    }
};


#endif //EDITOR_MYTEXTAREA_H