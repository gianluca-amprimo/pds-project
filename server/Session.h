
//
// Created by antonio on 08/08/20.
//

#ifndef SERVER_SESSION_H
#define SERVER_SESSION_H


#include "Symbol.h"
#include <QtCore/QVector>
#include <QtNetwork/QTcpSocket>
#include <memory>

#include "exceptions.h"

#define DEBUG false


/* A session represents an instance of a file that is open in at least an editor.
 * When a user opens a file, if the file is not open in any other editor, a new session will be created.
 * If the file is already being edited by someone else, the session will simply add
 * the user as an active collaborator in a list. The session also provides a clean interface
 * to communicate with collaborators, by keeping network sockets for each of them and other stats, like
 * current cursor position.
 */
class Session {

private:
    QList<Symbol> symbolsList;
    QHash<QString, std::shared_ptr<Symbol>> symbolsById;
    QMap<FracPosition, std::shared_ptr<Symbol>> symbolsByPosition;

private:
    QString filename;
    QMap<QChar, int> editorCurrentPosition;
public:
    const QMap<FracPosition, std::shared_ptr<Symbol>> &getSymbolsByPosition() const;


private:
    int editorCounter;
    QString editorPrefix;
public:
    int charCounter = 0;
    QHash<QString, QString> userMap;                         // utente - colore
    QHash<QString, QString> userEditorId;                    // utente -> editorid
    //void setSymbolsByPosition(const QMap<FracPosition, Symbol> &symbolsByPosition);

    Session(const QString &filename);

    const QString &getFilename() const;
    void setFilename(const QString &filename);

    int getEditorCounter() const;
    void setEditorCounter(int editorCounter);

    const QString &getEditorPrefix() const;
    void setEditorPrefix(const QString &editorPrefix);

    void addUserToSession(QString username, QString editorId);
    QString removeUserFromSession(const QString& username);

    void addSymbol(Symbol& sym);
    void removeSymbol(QString id);
    void changeSymbolFormat(QHash<QString, FracPosition> symbolsPosition, QTextCharFormat format);
    const void removeBatchSymbol(QHash<QString, FracPosition>& symbolsPosition);

    const QHash<QString, std::shared_ptr<Symbol>> &getSymbolsById() const;

};


#endif //SERVER_SESSION_H
