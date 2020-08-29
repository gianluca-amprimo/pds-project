//
// Created by antonio on 31/05/20.
//

#include "FracPosition.h"
#include <cmath>
#include <QtCore/QTextStream>
#include <QtCore/QDataStream>
#include <QDebug>

FracPosition &FracPosition::operator=(const FracPosition &fp) {
    this->position.clear();
    for (int i = 0; i < fp.position.size(); i++) {
        this->position.push_back(fp.position[i]);
    }
    this->stringify();
    return *this;
}

bool FracPosition::operator==(FracPosition fp) {
    bool equals = true;


    // if size of positions is different, no check required
    if (this->position.size() != fp.position.size()) {
        equals = false;
    } else {
        for (int i = 0; i < this->position.size(); i++) {
            if (this->position[i] != fp.position[i]) equals = false;
        }
    }
    return equals;
}

FracPosition FracPosition::operator+(const FracPosition &fp) {
    FracPosition result;
    int loopLen = this->position.size() <= fp.position.size() ? this->position.size() : fp.position.size();
    int lenDifference = std::abs(this->position.size() - fp.position.size());
    for (int i = 0; i < loopLen + lenDifference; i++) {
        result.position.push_back(0);
    }

    for (int i = loopLen - 1; i >= 0; i--) {
        result.position[i] += this->position[i] + fp.position[i];
        if (result.position[i] > 9 && i != 0) {
            result.position[i] -= 10;
            result.position[i - 1]++;
        }
    }

    for (int i = loopLen; i < loopLen + lenDifference; i++) {
        if (this->position.size() == loopLen + lenDifference) {
            result.position[i] = this->position[i];
        } else {
            result.position[i] = fp.position[i];
        }
    }
    result.stripTrailingZeros();

    return result;
}

FracPosition FracPosition::divideByTwo() {
    FracPosition result;
    for (int i = 0; i < this->position.size() + 1; i++) {
        result.position.push_back(0);
    }
    for (int i = 0; i < this->position.size(); i++) {
        result.position[i] += this->position[i] / 2;
        if (this->position[i] % 2) {
            result.position[i + 1] += 5;
        }
    }

    result.stripTrailingZeros();
    result.stringify();
    return result;
}

int FracPosition::operator[](int i) {
    return this->position[i];
}

void FracPosition::stringify() {
    stringPosition.clear();
    QTextStream ss(&stringPosition);
    if (this->position.size() == 1) {
        ss << this->position[0];
        return;

    }
    ss << this->position[0] << ",";

    for (int i = 1; i < this->position.size(); i++) {
        ss << this->position[i];
    }
}

FracPosition &FracPosition::operator=(const QString &fp) {
    this->position.clear();
    this->position.push_back(fp.split(QLatin1String(","))[0].toInt());
    if (fp.split(QLatin1String(",")).size() > 1) {
        QStringList decimalPart = fp.split(QLatin1String(","))[1].split(QLatin1String(""));
        for (int i = 1; i < decimalPart.size(); i++) {
            this->position.push_back(decimalPart[i].toInt());
        }
        this->stripTrailingZeros();

    }

    this->stringify();
    return *this;
}

FracPosition FracPosition::operator+(const QString &fp) {
    FracPosition addend = (const FracPosition &) fp;

    FracPosition result;
    int loopLen = this->position.size() <= addend.position.size() ? this->position.size() : addend.position.size();
    int lenDifference = std::abs(int(this->position.size() - addend.position.size()));
    for (int i = 0; i < loopLen + lenDifference; i++) {
        result.position.push_back(0);
    }

    for (int i = loopLen - 1; i > 0; i--) {
        result.position[i] += this->position[i] + addend.position[i];
        if (result.position[i] > 9 && i != 0) {
            result.position[i] -= 10;
            result.position[i - 1]++;
        }
    }

    for (int i = loopLen; i < loopLen + lenDifference; i++) {
        if (this->position.size() == loopLen + lenDifference) {
            result.position[i] = this->position[i];
        } else {
            result.position[i] = addend.position[i];
        }
    }

    result.stripTrailingZeros();

    return result;
}

bool FracPosition::operator<(const FracPosition &fp) const {
    for (int i = 0; i < this->position.size(); i++) {
        if (this->position[i] > fp.position[i]) {
            return false;
        } else if (this->position[i] < fp.position[i]) {
            return true;
        } else continue;
    }

    return fp.position.size() > this->position.size();
}

bool FracPosition::operator>(const FracPosition &fp) const {
    for (int i = 0; i < this->position.size(); i++) {
        if (this->position[i] < fp.position[i]) {
            return false;
        } else if (this->position[i] > fp.position[i]) {
            return true;
        } else continue;
    }

    return fp.position.size() < this->position.size();
}

bool FracPosition::operator>=(const FracPosition &fp) const {
    for (int i = 0; i < this->position.size(); i++) {
        if (this->position[i] < fp.position[i]) {
            return false;
        } else if (this->position[i] > fp.position[i]) {
            return true;
        } else continue;
    }

    if (fp.position.size() < this->position.size()) return true;
    return true;
}

bool FracPosition::operator<=(const FracPosition &fp) const {
    for (int i = 0; i < this->position.size(); i++) {
        if (this->position[i] > fp.position[i]) {
            return false;
        } else if (this->position[i] < fp.position[i]) {
            return true;
        } else continue;
    }

    if (fp.position.size() > this->position.size()) return true;
    return true;
}

void FracPosition::stripTrailingZeros() {
    for (int i = this->position.size() - 1; i > 0; i--) {
        if (this->position[i] == 0) {
            this->position.pop_back();
        } else break;
    }
}


FracPosition::FracPosition() {}

FracPosition::FracPosition(const QString &fp) {
    this->position.clear();
    this->position.push_back(fp.split(QLatin1String(","))[0].toInt());
    if (fp.split(QLatin1String(",")).size() > 1) {
        QStringList decimalPart = fp.split(QLatin1String(","))[1].split(QLatin1String(""));
        for (int i = 1; i < decimalPart.size(); i++) {
            this->position.push_back(decimalPart[i].toInt());
        }
        this->stripTrailingZeros();
    }
    this->stringify();
}

QDataStream &FracPosition::serialize(QDataStream &out) const {
    out << position;
    out << stringPosition;
    return out;
}


QDataStream &FracPosition::deserialize(QDataStream &in) {
    in >> position;
    in >> stringPosition;
    return in;
}


const QString &FracPosition::getStringPosition() const {
    return stringPosition;
}



