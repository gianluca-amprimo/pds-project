//
// Created by antonio on 08/08/20.
//

#ifndef SERVER_SESSION_H
#define SERVER_SESSION_H


#include "Symbol.h"
#include <QtCore/QVector>
#include <QtNetwork/QTcpSocket>
#include "User.h"

/* A session represents an instan
#include "Symbol.h"
#include <QtCore/QVector>
#include <QtNetwork/QTcpSocket>
#include "User.h"

/* A session represents an instance of a file that is open in at least an editor.
 * When a user opens a file, if the file is not open in any other editor, a new session will be created.
 * If the file is already being edited by someone else, the session will simply add
 * the user as an active collaborator in a list. The session also provides a clean interface
 * to communicate with collaborators, by keeping network sockets for each of them and other stats, like
 * current cursor position.
 */
class Session {

private:
    QMap<QString, Symbol> symbols;
    QMap<QString, QTcpSocket> connectedEditors; // per adesso io terrei semplicemente il socket del client
    QString filename;
    QMap<QChar, int> editorCurrentPosition;
    int editorCounter;
    QString editorPrefix;
public:
    const QMap<QString, &getSymbols() const;

    void setSymbols(const QMap<QString, &symbols);

    int getEditorCounter() const;

    void setEditorCounter(int editorCounter);

    const QString &getEditorPrefix() const;

    void setEditorPrefix(const QString &editorPrefix);

public:
    Session(const QString &filename);
    const QString &getFilename() const;
    void setFilename(const QString &filename);
    void addUserToSession(User u);
    void addSymbol(Symbol sym);
    void removeSymbol(QString id);
};


#endif //SERVER_SESSION_H
