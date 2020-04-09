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
    virtual ~User();

    bool operator==(const User &rhs) const;

    bool operator!=(const User &rhs) const;

    bool operator<(const User &rhs) const;

    bool operator>(const User &rhs) const;

    bool operator<=(const User &rhs) const;

    bool operator>=(const User &rhs) const;

private:
    QString username;
public:
    const QString &getUsername() const;

private:
    //lo username è un identificativo univoco (username== non sono possibili)
    QString propic; //path all’immagine del profilo



};


#endif //SERVER_USER_H
