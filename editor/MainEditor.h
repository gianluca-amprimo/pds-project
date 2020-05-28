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
    explicit MainEditor(QWidget *parent = nullptr, std::wstring editorIdentifier = L"AAAA");
    ~MainEditor() override;

private:
    std::wstring thisEditorIdentifier = L"AAAA";
    Ui::MainEditor *ui;
    SaveAsDialog *saveAsDialog;

    QFontComboBox *fontSelector;
    QComboBox *sizeSelector;
    int position;
    void initUI();
    void setupActions();

private slots:
    void Bold();
    void Italic();
    void Underline();
    void selectFont(const QString &font);
    void selectSize(const QString &size);
    void alignCenter();
    void alignLeft();
    void alignRight();
    void alignJustify();
    void updateCharFormat();
};


#endif //PIDIESSE_MAINEDITOR_H
