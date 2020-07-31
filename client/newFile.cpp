#include "newFile.h"
#include "ui_newfile.h"

newFile::newFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newFile)
{
    ui->setupUi(this);
}

newFile::~newFile()
{
    delete ui;
}
