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
    initUI();
    saveAsDialog = new SaveAsDialog(this, ui->textArea);

//    this->thisEditorIdentifier = editorIdentifier;
//    this->ui->textArea->setThisEditorIdentifier(editorIdentifier);

    QObject::connect(ui->saveAs, SIGNAL(triggered()), saveAsDialog, SLOT(exec()) );
    QObject::connect(saveAsDialog->ui->buttonBox, &QDialogButtonBox::accepted, saveAsDialog, [=](){saveAsDialog->setFileName(saveAsDialog->ui->lineEdit->text().toStdString());});
    //    --------
    ui->bold->setCheckable(true);
    QObject::connect(ui->bold, SIGNAL(triggered()), this, SLOT(Bold()));
    ui->italic->setCheckable(true);
    QObject::connect(ui->italic, SIGNAL(triggered()), this, SLOT(Italic()));
    ui->underline->setCheckable(true);
    QObject::connect(ui->underline, SIGNAL(triggered()), this, SLOT(Underline()));
    QObject::connect(fontSelector, &QComboBox::textActivated, this, &MainEditor::selectFont);
    QObject::connect(sizeSelector, &QComboBox::textActivated, this, &MainEditor::selectSize);
//    QObject::connect(fontSelector, &QFontComboBox::currentFontChanged, fontSelector, &QComboBox::setEditText);
//    QObject::connect(ui->textArea, &QTextEdit::textChanged, fontSelector, [this](const QFont &font){this->fontSelector->setEditText(font.toString());});
//    QObject::connect(fontSelector, &QFontComboBox::currentFontChanged, this, &MainEditor::correctFont);

//    QObject::connect(fontSelector, SIGNAL(textActivated()), this, SLOT(selectFont));
    ui->alignCenter->setCheckable(true);
    QObject::connect(ui->alignCenter, SIGNAL(triggered()), this, SLOT(alignCenter()));
    ui->alignLeft->setCheckable(true);
    QObject::connect(ui->alignLeft, SIGNAL(triggered()), this, SLOT(alignLeft()));
    ui->alignRight->setCheckable(true);
    QObject::connect(ui->alignRight, SIGNAL(triggered()), this, SLOT(alignRight()));
    ui->alignJustified->setCheckable(true);
    QObject::connect(ui->alignJustified, SIGNAL(triggered()), this, SLOT(alignJustify()));
//    QObject::connect(ui->alignJustified, &QAction::hovered, ui->alignJustified, &QAction::setChecked(true));

    position = 0;
    QObject::connect(ui->textArea, &QTextEdit::textChanged, this, &MainEditor::updateCharFormat);
}

//void MainEditor::correctFont(const QFont &font) {
//
//}

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
//    std::cout << "Format: " << ui->textArea->textCursor().charFormat().font().key() << std::endl;
//    qDebug() << ui->textArea->textCursor().charFormat().font().key();

//    QTextCharFormat format;
//    format.setFontFamily(font);
//    ui->textArea->mergeCurrentCharFormat(format);

//    ui->textArea->setFontFamily(font);
    QFont Font(font);
    ui->textArea->setCurrentFont(Font);

//    format.setFont(font);
//    ui->textArea->setCurrentFont(font);
//    ui->textArea->setEnabled(true);
//    ui->textArea->textCursor().setBlockCharFormat(format);
}

void MainEditor::selectSize(const QString &size) {
//    QTextCharFormat format;
//    format.setFontPointSize(size.toDouble());
//    ui->textArea->mergeCurrentCharFormat(format);
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
//        QTextCharFormat format;
        if (ui->textArea->currentCharFormat().fontWeight() == QFont::Bold) {
            ui->bold->setChecked(true);
        }
        else {
            ui->bold->setChecked(false);
        }
        ui->italic->setChecked(ui->textArea->currentCharFormat().fontItalic());
        ui->underline->setChecked(ui->textArea->currentCharFormat().fontUnderline());
//        fontSelector->setEditText(ui->textArea->currentCharFormat().fontFamily());
//        sizeSelector->setEditText(QString::number(ui->textArea->currentCharFormat().fontPointSize()));
//        fontSelector->setCurrentText(ui->textArea->currentCharFormat().fontFamily());
//        sizeSelector->setCurrentText(QString::number(ui->textArea->currentCharFormat().fontPointSize()));

        fontSelector->setCurrentText(ui->textArea->currentCharFormat().font().family());
        sizeSelector->setCurrentText(QString::number(ui->textArea->currentCharFormat().font().pointSize()));

//        std::cout << ui->textArea->currentCharFormat().fontFamily().toStdString() << " ";
//        std::cout << ui->textArea->currentCharFormat().fontPointSize() << std::endl;
    }
    position = ui->textArea->textCursor().position();
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

//    ui->textArea->setFontFamily("Helvetica");
//    ui->textArea->setFontPointSize(14);
//    fontSelector->setEditText(QString("Helvetica"));
//    sizeSelector->setEditText(QString::number(14));
//    fontSelector->setCurrentFont(QString("Helvetica"));
//    sizeSelector->setCurrentText(QString::number(14));
//    fontSelector->setCurrentText(ui->textArea->currentCharFormat().fontFamily());
//    sizeSelector->setCurrentText(QString::number(ui->textArea->currentCharFormat().fontPointSize()));

    QFont font("Helvetica");
    font.setPointSize(14);
    ui->textArea->setFont(font);
    fontSelector->setCurrentText(ui->textArea->currentCharFormat().font().family());
    sizeSelector->setCurrentText(QString::number(ui->textArea->currentCharFormat().font().pointSize()));

//    std::cout << ui->textArea->currentCharFormat().fontFamily().toStdString() << " ";
//    std::cout << ui->textArea->currentCharFormat().fontPointSize() << std::endl;
//    std::cout << ui->textArea->fontInfo().family().toStdString() << std::endl;
//    std::cout << ui->textArea->font().toString().toStdString() << std::endl;
}

MainEditor::~MainEditor() {
    delete ui;
}

