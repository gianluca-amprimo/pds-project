//
// Created by antonio on 27/03/20.
//

#ifndef PROVAQT_FILESAVER_H
#define PROVAQT_FILESAVER_H


#include <QtCore/QObject>
#include <QtWidgets/QPlainTextEdit>

class FileSaver : public QObject{
    Q_OBJECT
private:
    std::string fileName;

public slots:
    void save();
    void setFileName(std::string filename);

public:
    FileSaver(QPlainTextEdit *plainTextEdit);
    QPlainTextEdit *plainTextEdit;

};


#endif //PROVAQT_FILESAVER_H
