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
    std::string fileName = "untitled";

public slots:
    void save();
    void setFileName(std::string filename);

public:
    FileSaver(QTextEdit *textEdit);
    QTextEdit *textEdit;

};


#endif //PROVAQT_FILESAVER_H
