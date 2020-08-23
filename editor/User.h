//
// Created by gianluca on 06/04/20.
//

#ifndef SERVER_USER_H
#define SERVER_USER_H

#include <QString>
#include <QTcpSocket>
#include <list>
#include <QtGui/QPixmap>


class User {
public:
    User(QString username, QPixmap propic, QString name, QString surname);
    virtual ~User();

    bool operator==(const User &rhs) const;
    bool operator!=(const User &rhs) const;
    bool operator<(const User &rhs) const;
    bool operator>(const User &rhs) const;
    bool operator<=(const User &rhs) const;
    bool operator>=(const User &rhs) const;
    const QPixmap &getPropic() const;
    const QString &getUsername() const;

private:
    //lo username è un identificativo univoco (username== non sono possibili)
    QPixmap propic;
    QString username;
public:
    void setPropic(const QPixmap &propic);

private:
    QString name;
    QString surname;
public:
    const QString &getName() const;
    void setName(const QString &name);
    const QString &getSurname() const;
    void setSurname(const QString &surname);
    QColor getColor();

};


#endif //SERVER_USER_H
