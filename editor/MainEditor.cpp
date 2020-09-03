#include "MainEditor.h"
#include "client.h"
#include <QtWidgets/QDialogButtonBox>
#include <iostream>
#include <QDebug>
#include <QToolButton>


MainEditor::MainEditor(QWidget *parent, QString editorIdentifier, QString filename,
                       QTcpSocket *tcpSocket, QDataStream *contentSteam, QString username) :
    QMainWindow(parent),
    ui(new Ui::MainEditor),
    filename(filename),
    tcpSocket(tcpSocket),
    username(username) {
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
    QObject::connect(this->textArea, &MyTextArea::charInserted, this, &MainEditor::sendCharInserted);
    QObject::connect(this->textArea, &MyTextArea::charDeleted, this, &MainEditor::sendCharDeleted);
    QObject::connect(this->textArea, &MyTextArea::batchCharDelete, this, &MainEditor::sendBatchCharDeleted);
    QObject::connect(this->textArea, &MyTextArea::batchCharInserted, this, &MainEditor::sendBatchCharInserted);

    // TODO: create a timer to send periodically the position of the user
    // create a timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainEditor::sendPosition);
    timer->start(2000);
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
        message["username"] = this->username;
        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        if (!tcpSocket->write(block)) {
            QMessageBox::information(this, tr("PdS Server"), tr("Could not send message.\nTry again later."));
        }
        tcpSocket->flush();
    }
    qDebug() << "Message session log out sent, waiting for reply...";
    timer->stop();
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

void MainEditor::initUI(QDataStream *contentStream) {
    textArea = new MyTextArea(ui->centralwidget);
    textArea->setObjectName(QString::fromUtf8("textArea"));
    ui->gridLayout->addWidget(textArea, 1, 0, 1, 1);

    QList<Symbol> listOfSymbols;
    *contentStream >> listOfSymbols;
    for(const auto& sym : listOfSymbols){
        textArea->addSymbolToList(sym);
    }
    for(const auto& sym : textArea->getSymbols().values()){
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
        outFileStream << this->textArea->getSymbols().values();
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

void MainEditor::sendCharInserted(QJsonObject message) {
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

        // add filename to message
        message["filename"] = this->filename;
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        if (!this->tcpSocket->write(block)) {
            ui->statusBar->showMessage(tr("Could not send message to the server, char will be not inserted."), 5000);
        }
        this->tcpSocket->flush();
        qDebug() << "Sending insertion of char " << message["unicode"] << "at position" << message["position"];
    }

}

void MainEditor::sendCharDeleted(QJsonObject message) {
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

        message["filename"] = this->filename;

        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        if (!this->tcpSocket->write(block)) {
            ui->statusBar->showMessage(tr("Could not save the file.\nTry again later."), 5000);
        }
        this->tcpSocket->flush();
        qDebug() << "Sending deletion of" << message["charId"];
    }
}

void MainEditor::receiveSymbol(QJsonValueRef content) {
    auto data = QByteArray::fromBase64(content.toString().toLatin1());
    QDataStream inStream(&data, QIODevice::ReadOnly);
    Symbol sym;

    inStream >> sym;
    this->textArea->addSymbolToList(sym);
}

void MainEditor::receiveDeletion(QJsonValueRef id, QJsonValueRef position) {
    auto symId = id.toString();
    auto symPos = position.toString();

    qDebug() << "Received the deletion of char" << symId << "at position" << symPos;
    this->textArea->removeSymbolFromList(symId, symPos);
}

const QString &MainEditor::getFilename() const {
    return filename;
}

void MainEditor::sendBatchCharDeleted(QJsonObject message) {
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

        message["filename"] = this->filename;

        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        if (!this->tcpSocket->write(block)) {
            ui->statusBar->showMessage(tr("Could not save the file.\nTry again later."), 5000);
        }
        this->tcpSocket->flush();
    }

}

void MainEditor::receiveBatchDeletion(QJsonValueRef idsAndPositionsJson) {
    auto idsPositionsBytes = QByteArray::fromBase64(idsAndPositionsJson.toString().toLatin1());
    QDataStream inIdsPositionBytesStream(&idsPositionsBytes, QIODevice::ReadOnly);

    QHash<QString, FracPosition> idsAndPositions;
    inIdsPositionBytesStream >> idsAndPositions;

    for(auto key : idsAndPositions.keys()){
        this->textArea->removeSymbolFromList(key, const_cast<QString &>(idsAndPositions.value(key).getStringPosition()));
    }
}

void MainEditor::sendBatchCharInserted(QJsonArray message, QVector<QTextCharFormat> formats) {

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
        QByteArray formatInBytes;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);

        QDataStream formatOut(&formatInBytes, QIODevice::WriteOnly);
        formatOut.setVersion(QDataStream::Qt_4_0);
        formatOut << formats;

        QJsonObject metadata;
        metadata["filename"] = this->filename;
        metadata["editorId"] = this->textArea->getThisEditorIdentifier();
        metadata["charLength"] = message.size();
        metadata["formatLength"] = 0;
        metadata["length"] = message.size();
        metadata["header"] = BATCH_CHAR_ADDITION;
        metadata["formats"] = QLatin1String(formatInBytes.toBase64());

        message.push_front(metadata);

        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        if (!this->tcpSocket->write(block)) {
            ui->statusBar->showMessage(tr("Could not save the file.\nTry again later."), 5000);
        }
        this->tcpSocket->flush();
    }
}

void MainEditor::receiveBatchSymbol(QJsonArray data) {
    QJsonObject metadata = data[0].toObject();

    auto formatsInBytes = QByteArray::fromBase64(metadata["formats"].toString().toLatin1());
    QVector<QTextCharFormat> formats;
    QDataStream inFormatsStream(&formatsInBytes, QIODevice::ReadOnly);
    inFormatsStream >> formats;

    int arrayLength = metadata["length"].toInt();
    qDebug() << "Going to paste" << arrayLength << "characters";

    for(int i = 0; i < arrayLength; i++){
        QJsonObject singleSymbol = data[i+1].toObject();
        FracPosition fp(singleSymbol["fracPosition"].toString());
        QChar unicode = singleSymbol["unicode"].toString()[0];
        QString charId = singleSymbol["charId"].toString();
        QTextCharFormat format = formats[0];

        Symbol sym(unicode, charId, fp, format);
        qDebug() << "Batch inserting" << sym.getCharacter() << "at position" << sym.getPosition().getStringPosition();
         this->textArea->addSymbolToList(sym);
    }
}

void MainEditor::colors(QString username, QString color, QString postion){
    // TODO: print grafically the users with their color in their position
    qDebug() << "User: " << username << " has color: " << color << " and is at position: " << position;
}

void MainEditor::sendPosition(){
    QString userPosition= QString::number(this->textArea->getCurrentPosition());
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
        message["header"] = "position";
        message["username"] = this->username;
        message["filename"] = this->filename;
        message["position"] = userPosition;

        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        if (!tcpSocket->write(block)) {
            QMessageBox::information(this, tr("PdS Server"), tr("Could not send message.\nTry again later."));
        }
        tcpSocket->flush();
    }
}
