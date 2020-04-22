#ifndef PIDIESSE_MAINEDITOR_H
#define PIDIESSE_MAINEDITOR_H

#include <QMainWindow>
#include "ui_MainEditor.h"
#include "SaveAsDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainEditor; }
QT_END_NAMESPACE

class MainEditor : public QMainWindow {
    Q_OBJECT
public:
    explicit MainEditor(QWidget *parent = nullptr);
    ~MainEditor() override;
private:
    void initIcons();
    Ui::MainEditor *ui;
    SaveAsDialog *saveAsDialog;
};


#endif //PIDIESSE_MAINEDITOR_H
