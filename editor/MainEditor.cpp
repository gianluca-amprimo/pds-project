#include "MainEditor.h"
#include "client.h"
#include <QtWidgets/QDialogButtonBox>
#include <iostream>
#include <QDebug>
#include <QToolButton>


MainEditor::MainEditor(QWidget *parent, QString editorIdentifier, QString filename, QTcpSocket *tcpSocket, QDataStream *contentSteam) :
    QMainWindow(parent),
    ui(new Ui::MainEditor),
    filename(filename),
    tcpSocket(tcpSocket) {
    ui->setupUi(this);
    initUI(contentSteam);
    this->setWindowTitle("PiDiEsse - " + filename);
    setupActions();
    saveAsDialog = new SaveAsDialog(this, this->textArea);

    this->textArea->setThisEditorIdentifier(editorIdentifier);
    qDebug() << "Starting with ID " + this->textArea->getThisEditorIdentifier();
//  this->ui->textArea->setThisEditorIdentifier(editorIdentifier);


    QObject::connect(ui->saveAs, SIGNAL(triggered()), saveAsDialog, SLOT(exec()) );
    QObject::connect(saveAsDialog->ui->buttonBox, &QDialogButtonBox::accepted, saveAsDialog, [=](){saveAsDialog->setFileName(saveAsDialog->ui->lineEdit->text().toStdString());});
    QObject::connect(this->textArea, &MyTextArea::symbolReady, this, &MainEditor::sendSymbol);
    QObject::connect(this->textArea, &MyTextArea::symbolDeleted, this, &MainEditor::sendDeletion);
}

void MainEditor::closeEvent(QCloseEvent *event) {
    auto client = qobject_cast<Client *>(this->parent());
    client->getChoiceWin()->setVisible(true);
    event->accept();

    qDebug() << "Sending message to disconnect client from session from the server.";
    if (tcpSocket != nullptr) {
        if (!tcpSocket->isValid()) {
            qDebug() << "tcp socket invalid";
            return;
        }
        if (!tcpSocket->isOpen()) {
            qDebug() << "tcp socket not open";
            return;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        QJsonObject message;
        message["header"] = "sessionlogout";
        message["filename"] = this->filename;
        message["editorId"] = this->textArea->getThisEditorIdentifier();
        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        if (!tcpSocket->write(block)) {
            QMessageBox::information(this, tr("PdS Server"), tr("Could not send message.\nTry again later."));
        }
        tcpSocket->flush();
    }
    qDebug() << "Message session log out sent, waiting for reply...";
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

    //QObject::connect(fontSelector, SIGNAL(textActivated()), this, SLOT(selectFont()));
    //QObject::connect(sizeSelector, SIGNAL(textActivated()), this, SLOT( selectSize()));

    ui->alignCenter->setCheckable(true);
    QObject::connect(ui->alignCenter, SIGNAL(triggered()), this, SLOT(alignCenter()));
    ui->alignLeft->setCheckable(true);
    QObject::connect(ui->alignLeft, SIGNAL(triggered()), this, SLOT(alignLeft()));
    ui->alignRight->setCheckable(true);
    QObject::connect(ui->alignRight, SIGNAL(triggered()), this, SLOT(alignRight()));
    ui->alignJustified->setCheckable(true);
    QObject::connect(ui->alignJustified, SIGNAL(triggered()), this, SLOT(alignJustify()));

    position = 0;
    QObject::connect(this->textArea, &QTextEdit::textChanged, this, &MainEditor::updateCharFormat);
    QObject::connect(ui->save, SIGNAL(triggered()), this, SLOT(save()));
}

void MainEditor::initUI(QDataStream *contentSteam) {
    textArea = new MyTextArea(ui->centralwidget);
    textArea->setObjectName(QString::fromUtf8("textArea"));
    ui->gridLayout->addWidget(textArea, 1, 0, 1, 1);

    *contentSteam >> *textArea;
    for(auto sym : textArea->getSymbols()){
        textArea->setCurrentCharFormat(sym.getCharFormat());
        textArea->insertPlainText(sym.getCharacter());
    }

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
    this->textArea->setFont(font);
    this->textArea->setAlignment(Qt::AlignLeft);
    fontSelector->setCurrentText(this->textArea->currentCharFormat().font().family());
    sizeSelector->setCurrentText(QString::number(this->textArea->currentCharFormat().font().pointSize()));
    ui->alignLeft->setChecked(true);
}


void MainEditor::Bold() {
    QTextCharFormat format;
    format.setFontWeight(ui->bold->isChecked() ? QFont::Bold : QFont::Normal);
    this->textArea->mergeCurrentCharFormat(format);
}

void MainEditor::Italic() {
    QTextCharFormat format;
    format.setFontItalic(ui->italic->isChecked());
    this->textArea->mergeCurrentCharFormat(format);
}

void MainEditor::Underline() {
    QTextCharFormat format;
    format.setFontUnderline(ui->underline->isChecked());
    this->textArea->mergeCurrentCharFormat(format);
}

void MainEditor::selectFont(const QString &font) {
    QTextCharFormat format;
    format.setFontFamily(font);
    this->textArea->mergeCurrentCharFormat(format);
}

void MainEditor::selectSize(const QString &size) {
    this->textArea->setFontPointSize(size.toDouble());
}

void MainEditor::alignCenter() {
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(ui->alignCenter->isChecked() ? Qt::AlignHCenter : Qt::AlignLeft);
    ui->alignLeft->setChecked(false);
    ui->alignRight->setChecked(false);
    ui->alignJustified->setChecked(false);
    this->textArea->textCursor().setBlockFormat(blockFormat);
}

void MainEditor::alignLeft() {
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignLeft);
    ui->alignCenter->setChecked(false);
    ui->alignRight->setChecked(false);
    ui->alignJustified->setChecked(false);
    this->textArea->textCursor().setBlockFormat(blockFormat);
}

void MainEditor::alignRight() {
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(ui->alignRight->isChecked() ? Qt::AlignRight : Qt::AlignLeft);
    ui->alignLeft->setChecked(false);
    ui->alignCenter->setChecked(false);
    ui->alignJustified->setChecked(false);
    this->textArea->textCursor().setBlockFormat(blockFormat);
}

void MainEditor::alignJustify() {
    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(ui->alignJustified->isChecked() ? Qt::AlignJustify : Qt::AlignLeft);
    ui->alignLeft->setChecked(false);
    ui->alignCenter->setChecked(false);
    ui->alignRight->setChecked(false);
    this->textArea->textCursor().setBlockFormat(blockFormat);
}

void MainEditor::updateCharFormat() {
    if (position > this->textArea->textCursor().position()) {
        ui->bold->setChecked(this->textArea->currentCharFormat().fontWeight() == QFont::Bold);
        ui->italic->setChecked(this->textArea->currentCharFormat().fontItalic());
        ui->underline->setChecked(this->textArea->currentCharFormat().fontUnderline());

        fontSelector->setCurrentText(this->textArea->currentCharFormat().font().family());
        sizeSelector->setCurrentText(QString::number(this->textArea->currentCharFormat().font().pointSize()));
    }
    position = this->textArea->textCursor().position();
}

void MainEditor::save() {
    if (this->tcpSocket != nullptr) {
        if (!this->tcpSocket->isValid()) {
            qDebug() << "tcp socket invalid";
            return;
        }
        if (!this->tcpSocket->isOpen()) {
            qDebug() << "tcp socket not open";
            return;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);

        QJsonObject message;
        message["header"] = "savefile";
        message["filename"] = this->filename;

        QByteArray byteArrayBuffer;
        QDataStream outFileStream(&byteArrayBuffer, QIODevice::WriteOnly);
        outFileStream << *this->textArea;
        message["content"] = QLatin1String(byteArrayBuffer.toBase64());

        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        if (!this->tcpSocket->write(block)) {
            ui->statusBar->showMessage(tr("Could not save the file.\nTry again later."), 5000);
        }
        this->tcpSocket->flush();
    }
    qDebug() << "Saving file " + filename;
}

Ui::MainEditor *MainEditor::getUi() {
    return this->ui;
}

void MainEditor::sendSymbol(Symbol& symbol) {
    // check socket status
    if (this->tcpSocket != nullptr) {
        if (!this->tcpSocket->isValid()) {
            qDebug() << "tcp socket invalid";
            return;
        }
        if (!this->tcpSocket->isOpen()) {
            qDebug() << "tcp socket not open";
            return;
        }

        QByteArray serializedSym;
        QDataStream symbolStream(&serializedSym, QIODevice::WriteOnly);
        symbolStream << symbol;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);

        QJsonObject message;
        message["header"] = "symbol";
        message["filename"] = this->filename;
        message["editorId"] = this->textArea->getThisEditorIdentifier();
        message["content"] = QLatin1String(serializedSym.toBase64());

        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        if (!this->tcpSocket->write(block)) {
            ui->statusBar->showMessage(tr("Could not save the file.\nTry again later."), 5000);
        }
        this->tcpSocket->flush();
        qDebug() << "Sending symbol " << symbol.getCharacter() << "at position" << symbol.getPosition().getStringPosition();
    }

}

void MainEditor::receiveSymbol(QJsonValueRef content) {

    auto data = QByteArray::fromBase64(content.toString().toLatin1());
    QDataStream in(data);
    Symbol sym;

    in >> sym;
    this->textArea->addSymbolToList(sym);
    QTextCursor cur = this->textArea->textCursor();
    cur.setPosition(this->textArea->getEditorPosition(sym.getPosition()));
    cur.insertText(QString(sym.getCharacter()), sym.getCharFormat());
}

void MainEditor::sendDeletion(QByteArray serializedSymId) {
    if (this->tcpSocket != nullptr) {
        if (!this->tcpSocket->isValid()) {
            qDebug() << "tcp socket invalid";
            return;
        }
        if (!this->tcpSocket->isOpen()) {
            qDebug() << "tcp socket not open";
            return;
        }

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);

        QJsonObject message;
        message["header"] = "delSymbol";
        message["filename"] = this->filename;
        message["editorId"] = this->textArea->getThisEditorIdentifier();

        message["content"] = QLatin1String(serializedSymId.toBase64());

        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        if (!this->tcpSocket->write(block)) {
            ui->statusBar->showMessage(tr("Could not save the file.\nTry again later."), 5000);
        }
        //this->tcpSocket->flush();
        qDebug() << "Sending deletion of" << message["content"];
    }
}

void MainEditor::receiveDeletion(QJsonValueRef id, QJsonValueRef position) {
    auto symId = QByteArray::fromBase64(id.toString().toLatin1());
    auto symPos = position.toString();
    QDataStream idStream(symId);
    QString idString;


    idStream >> idString;
    qDebug() << "Received the deletion of char" << idString << "at position" << symPos;
    this->textArea->removeSymbolFromList(idString, symPos);

}



