//
// Created by antonio on 27/03/20.
//

#ifndef PROVAQT_FILESAVER_H
#define PROVAQT_FILESAVER_H

#include <QDialog>
#include "ui_SaveAsDialog.h"
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class SaveAsDialog; }
QT_END_NAMESPACE

class SaveAsDialog : public QDialog{
    Q_OBJECT

public:
    explicit SaveAsDialog(QWidget *mainEditor = nullptr, QTextEdit *textArea = nullptr);
    ~SaveAsDialog() override;
    Ui::SaveAsDialog *ui;

public slots:
    void save();
    void setFileName(std::string filename);

private:
    QTextEdit *textArea;
    std::string fileName = "untitled";
};


#endif //PROVAQT_FILESAVER_H
