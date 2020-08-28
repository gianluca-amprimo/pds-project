
//
// Created by antonio on 08/08/20.
//

#ifndef SERVER_SESSION_H
#define SERVER_SESSION_H


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
    QHash<QString, Symbol> symbols;
    QString filename;
    QMap<QChar, int> editorCurrentPosition;
    int editorCounter;
    QString editorPrefix;
public:
    QMap<QString, User*> connectedEditors; // per adesso io terrei semplicemente il socket del client

    int getEditorCounter() const;
    void setEditorCounter(int editorCounter);
    const QString &getEditorPrefix() const;
    void setEditorPrefix(const QString &editorPrefix);

    // mappa nome utente - colore_posizione
    QMap<QString, QString> userMap;

public:
    Session(const QString &filename);
    const QString &getFilename() const;
    void setFilename(const QString &filename);
    void addUserToSession(User *u);
    void removeUserFromSession(User *u);
    void addSymbol(Symbol& sym);
    void removeSymbol(QString id);

    const QHash<QString, Symbol> &getSymbols() const;
};


#endif //SERVER_SESSION_H
