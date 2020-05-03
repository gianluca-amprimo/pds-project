#ifndef PIDIESSE_MAINEDITOR_H
#define PIDIESSE_MAINEDITOR_H

#include <QMainWindow>
#include <QtWidgets/QFontComboBox>
#include "ui_MainEditor.h"
#include "SaveAsDialog.h"
#include "Symbol.h"

#define HEAD    0
#define BACK    1
#define MIDDLE  2

QT_BEGIN_NAMESPACE
namespace Ui { class MainEditor; }
QT_END_NAMESPACE

class MainEditor : public QMainWindow {
    Q_OBJECT
public:
    explicit MainEditor(QWidget *parent = nullptr, std::string editorIdentifier = "AAAA");
    ~MainEditor() override;

public slots:
    void notifyChange();
    void notifyMove();

private:
    std::string thisEditorIdentifier = "AAAA";
    int charCounter;
    std::vector<Symbol> _symbols;
    Ui::MainEditor *ui;
private:
    SaveAsDialog *saveAsDialog;
    int currentPosition;
    int oldPosition;

    QFontComboBox *fontSelector;
    void initUI();
};


#endif //PIDIESSE_MAINEDITOR_H
