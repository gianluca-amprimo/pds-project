//
// Created by antonio on 31/05/20.
//

#ifndef EDITOR_FRACPOSITION_H
#define EDITOR_FRACPOSITION_H


#include <vector>
#include <QVector>

class FracPosition {

private:
    QVector<int> position;
    void stripTrailingZeros();

    QString stringPosition;

    QDataStream& serialize(QDataStream& out) const;
    QDataStream& deserialize(QDataStream& in);

    void stringify();

public:
    const QString &getStringPosition() const;
    bool operator==(FracPosition fp);
    bool operator<(const FracPosition &fp) const;
    bool operator>(const FracPosition &fp) const;
    bool operator<=(const FracPosition &fp) const;
    bool operator>=(const FracPosition &fp) const;
    FracPosition operator+(const FracPosition &fp);
    FracPosition operator+(const QString &fp);
    FracPosition& operator=(const FracPosition &fp);
    FracPosition& operator=(const QString &fp);

    friend QDataStream& operator<<(QDataStream& out, FracPosition const& pos) {
        return pos.serialize(out);
    }

    friend QDataStream& operator>>(QDataStream& in, FracPosition& pos) {
        return  pos.deserialize(in);
    }

    static const FracPosition& one();

    FracPosition(const QString &fp);
    FracPosition();

    int operator[](int i);
    FracPosition divideByTwo();
};

static FracPosition one("1");

#endif //EDITOR_FRACPOSITION_H
