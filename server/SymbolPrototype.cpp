#include <QFont>
#include <QIODevice>
#include <QDataStream>
#include <QMap>
#include <iostream>
#include <QFile>

class SymbolPrototype {
private:
    QChar ch;
    float position;
    QFont style;

public:
    SymbolPrototype(){
        this->ch = '0';
        this->position = 0;
        this->style = QFont();
    }

    SymbolPrototype(char ch, float position) {
        this->ch = QChar(ch);
        this->position = position;
        this->style = QFont();
    }

    virtual ~SymbolPrototype() = default;

    QChar getCh(){
        return this->ch;
    }


    QDataStream& serialize(QDataStream& out) const {
        out << ch;
        out << position;
        out << style;
        return out;
    }

    QDataStream& deserialize(QDataStream& in) {
        in >> ch;
        in >> position;
        in >> style;
        return in;
    }

    friend QDataStream& operator<<(QDataStream& out, SymbolPrototype const& sym) {
        return sym.serialize(out);
    }

    friend QDataStream& operator>>(QDataStream& in, SymbolPrototype& sym) {
        return  sym.deserialize(in);
    }
};

class FilePrototype {
private:
    QMap<float, SymbolPrototype> listOfSymbols;

public:
    FilePrototype() {
        this->listOfSymbols = QMap<float, SymbolPrototype>();
    }

    virtual ~FilePrototype() = default;

    void addSymbol(float position, char ch) {
        SymbolPrototype sym = SymbolPrototype(ch, position);
        this->listOfSymbols.insert(position, sym);
    }

    QDataStream& serialize(QDataStream& out) const {
        out << listOfSymbols;
        return out;
    }

    QDataStream& deserialize(QDataStream& in) {
        in >> listOfSymbols;
        return in;
    }

    friend QDataStream& operator<<(QDataStream& out, FilePrototype const& fProto) {
        return fProto.serialize(out);
    }

    friend QDataStream& operator>>(QDataStream& in, FilePrototype& fProto) {
        return  fProto.deserialize(in);
    }


    friend std::ostream& operator<<(std::ostream& out, FilePrototype& fProto) {
      // print something from v to str, e.g: Str << v.getX();
      for(SymbolPrototype sym : fProto.listOfSymbols.values()){
          out << sym.getCh().toLatin1();
      }
      return out;
    }
};
