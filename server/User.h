//
// Created by gianluca on 06/04/20.
//

#ifndef SERVER_USER_H
#define SERVER_USER_H

#include <QString>
#include <QTcpSocket>
#include <list>



class User {
public:
    User(QString username);

    User(const User &user);

    virtual ~User();

    bool operator==(const User &rhs) const;

    bool operator!=(const User &rhs) const;

    bool operator<(const User &rhs) const;

    bool operator>(const User &rhs) const;

    bool operator<=(const User &rhs) const;

    bool operator>=(const User &rhs) const;
    
    const QString &getUsername() const;

    const QString &getEditorId() const;

    void setEditorId(const QString &editorId);

private:
	QString username;           // lo username è un identificativo univoco (username== non sono possibili)
	QString editorId;
    QString propic;             // path all’immagine del profilo
};


#endif //SERVER_USER_H
