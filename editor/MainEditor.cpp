#include "MainEditor.h"
#include <QtWidgets/QDialogButtonBox>
#include <iostream>
#include <iomanip>
#include <QDebug>
#include <QToolButton>


MainEditor::MainEditor(QWidget *parent, std::string editorIdentifier) :
    QMainWindow(parent),
    ui(new Ui::MainEditor) {
    ui->setupUi(this);
    setupActions();
    initUI();
    saveAsDialog = new SaveAsDialog(this, ui->textArea);

    this->thisEditorIdentifier = editorIdentifier;
//    this->ui->textArea->setThisEditorIdentifier(editorIdentifier);

    QObject::connect(ui->saveAs, SIGNAL(triggered()), saveAsDialog, SLOT(exec()) );
    QObject::connect(saveAsDialog->ui->buttonBox, &QDialogButtonBox::accepted, saveAsDialog, [=](){saveAsDialog->setFileName(saveAsDialog->ui->lineEdit->text().toStdString());});
}

MainEditor::~MainEditor() {
    delete ui;
}

void MainEditor::setupActions() {
    ui->bold->setCheckable(true);
    QObject::connect(ui->bold, SIGNAL(triggered()), this, SLOT(Bold()));
    ui->italic->setCheckable(true);
    QObject::connect(ui->italic, SIGNAL(triggered()), this, SLOT(Italic()));
    ui->underline->setCheckable(true);
    QObject::connect(ui->underline, SIGNAL(triggered()), this, SLOT(Underline()));

    QObject::connect(fontSelector, &QComboBox::textActivated, this, &MainEditor::selectFont);
    QObject::connect(sizeSelector, &QComboBox::textActivated, this, &MainEditor::selectSize);

    ui->alignCenter->setCheckable(true);
    QObject::connect(ui->alignCenter, SIGNAL(triggered()), this, SLOT(alignCenter()));
    ui->alignLeft->setCheckable(true);
    QObject::connect(ui->alignLeft, SIGNAL(triggered()), this, SLOT(alignLeft()));
    ui->alignRight->setCheckable(true);
    QObject::connect(ui->alignRight, SIGNAL(triggered()), this, SLOT(alignRight()));
    ui->alignJustified->setCheckable(true);
    QObject::connect(ui->alignJustified, SIGNAL(triggered()), this, SLOT(alignJustify()));

    position = 0;
    QObject::connect(ui->textArea, &QTextEdit::textChanged, this, &MainEditor::updateCharFormat);
}

void MainEditor::initUI() {
    auto separator = ui->toolBar->insertSeparator(ui->bold);
    this->fontSelector = new QFontComboBox;
    ui->toolBar->insertWidget(separator, fontSelector);
    this->fontSelector->setFocusPolicy(Qt::NoFocus);

    QList<int> sizes = QFontDatabase::standardSizes();
    this->sizeSelector = new QComboBox;
    for (int size: sizes) {
        this->sizeSelector->addItem(QString::number(size));
    }
    ui->toolBar->insertWidget(separator, sizeSelector);
    this->sizeSelector->setFocusPolicy(Qt::NoFocus);

    QFont font("Helvetica");
    font.setPointSize(14);
    ui->textArea->setFont(font);
    ui->textArea->setAlignment(Qt::AlignLeft);
    fontSelector->setCurrentText(ui->textArea->currentCharFormat().font().family());
    sizeSelector->setCurrentText(QString::number(ui->textArea->currentCharFormat().font().pointSize()));
    ui->alignLeft->setChecked(true);
}


void MainEditor::Bold() {
    QTextCharFormat format;
    format.setFontWeight(ui->bold->isChecked() ? QFont::Bold : QFont::Normal);
    ui->textArea->mergeCurrentCharFormat(format);
}

void MainEditor::Italic() {
    QTextCharFormat format;
    format.setFontItalic(ui->italic->isChecked());
    ui->textArea->mergeCurrentCharFormat(format);
}

void MainEditor::Underline() {
    QTextCharFormat format;
    format.setFontUnderline(ui->underline->isChecked());
    ui->textArea->mergeCurrentCharFormat(format);
}

void MainEditor::selectFont(const QString &font) {
    QFont Font(font);
    ui->textArea->setCurrentFont(Font);
}

void MainEditor::selectSize(const QString &size) {
    ui->textArea->setFontPointSize(size.toDouble());
}

void MainEditor::alignCenter() {
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(ui->alignCenter->isChecked() ? Qt::AlignHCenter : Qt::AlignLeft);
    ui->alignLeft->setChecked(false);
    ui->alignRight->setChecked(false);
    ui->alignJustified->setChecked(false);
    ui->textArea->textCursor().setBlockFormat(blockFormat);
}

void MainEditor::alignLeft() {
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignLeft);
    ui->alignCenter->setChecked(false);
    ui->alignRight->setChecked(false);
    ui->alignJustified->setChecked(false);
    ui->textArea->textCursor().setBlockFormat(blockFormat);
}

void MainEditor::alignRight() {
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(ui->alignRight->isChecked() ? Qt::AlignRight : Qt::AlignLeft);
    ui->alignLeft->setChecked(false);
    ui->alignCenter->setChecked(false);
    ui->alignJustified->setChecked(false);
    ui->textArea->textCursor().setBlockFormat(blockFormat);
}

void MainEditor::alignJustify() {
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(ui->alignJustified->isChecked() ? Qt::AlignJustify : Qt::AlignLeft);
    ui->alignLeft->setChecked(false);
    ui->alignCenter->setChecked(false);
    ui->alignRight->setChecked(false);
    ui->textArea->textCursor().setBlockFormat(blockFormat);
}

void MainEditor::updateCharFormat() {
    if (position > ui->textArea->textCursor().position()) {
        if (ui->textArea->currentCharFormat().fontWeight() == QFont::Bold) {
            ui->bold->setChecked(true);
        }
        else {
            ui->bold->setChecked(false);
        }
        ui->italic->setChecked(ui->textArea->currentCharFormat().fontItalic());
        ui->underline->setChecked(ui->textArea->currentCharFormat().fontUnderline());

        fontSelector->setCurrentText(ui->textArea->currentCharFormat().font().family());
        sizeSelector->setCurrentText(QString::number(ui->textArea->currentCharFormat().font().pointSize()));
    }
    position = ui->textArea->textCursor().position();
}


