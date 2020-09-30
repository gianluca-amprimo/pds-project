#include "MainEditor.h"
#include "client.h"
#include <QtWidgets/QDialogButtonBox>
#include <iostream>
#include <QDebug>

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

    this->textArea->setThisEditorIdentifier(editorIdentifier);
#if DEBUG
    qDebug() << "Starting with ID " + this->textArea->getThisEditorIdentifier();
#endif
    //  this->ui->textArea->setThisEditorIdentifier(editorIdentifier);

    QObject::connect(ui->exportAsPDF, SIGNAL(triggered()), this, SLOT(exportAsPDF()));
    QObject::connect(this->textArea, &MyTextArea::charInserted, this, &MainEditor::sendCharInserted);
    QObject::connect(this->textArea, &MyTextArea::charDeleted, this, &MainEditor::sendCharDeleted);
    QObject::connect(this->textArea, &MyTextArea::batchCharDelete, this, &MainEditor::sendBatchCharDeleted);
    QObject::connect(this->textArea, &MyTextArea::batchCharInserted, this, &MainEditor::sendBatchCharInserted);
    QObject::connect(this->textArea, &MyTextArea::formatCharChanged, this, &MainEditor::sendCharFormatChanged);
}

void MainEditor::closeEvent(QCloseEvent *event) {
    auto client = qobject_cast<Client *>(this->parent());
    client->getChoiceWin()->setVisible(true);
    event->accept();

#if DEBUG
    qDebug() << "Sending message to disconnect client from session from the server.";
#endif

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
#if DEBUG
    qDebug() << "Message session log out sent, waiting for reply...";
#endif
}

MainEditor::~MainEditor() {
    delete ui;
}

void MainEditor::setupActions() {
    ui->bold->setCheckable(true);
    QObject::connect(ui->bold, SIGNAL(triggered()), this, SLOT(Bold()));
    QObject::connect(this->textArea, SIGNAL(boldFormatActivate()), this, SLOT(BoldShortcut()));
    ui->italic->setCheckable(true);
    QObject::connect(ui->italic, SIGNAL(triggered()), this, SLOT(Italic()));
    QObject::connect(this->textArea, SIGNAL(italicFormatActivate()), this, SLOT(ItalicShortcut()));
    ui->underline->setCheckable(true);
    QObject::connect(ui->underline, SIGNAL(triggered()), this, SLOT(Underline()));
    QObject::connect(this->textArea, SIGNAL(underlineFormatActivate()), this, SLOT(UnderlineShortcut()));
    //QObject::connect(fontSelector, SIGNAL(textActivated()), this, SLOT(selectFont()));
    //QObject::connect(sizeSelector, SIGNAL(textActivated()), this, SLOT( selectSize()));
    QObject::connect(this->textArea, &MyTextArea::currentCharFormatChanged, this, &MainEditor::updateStyleButton);

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
    // initialising the scroll bar were we will place the online users
    onlineUsers = new QListWidget(ui->centralwidget);
    onlineUsers->setObjectName(QString::fromUtf8("onlineUsers"));
    QString line = "Here online users \nwill be displayed";
    onlineUsers->addItem(line);
    ui->gridLayout->addWidget(onlineUsers, 0, 0, 1, -1);

    // initialising the text area
    textArea = new MyTextArea(ui->centralwidget);
    textArea->setObjectName(QString::fromUtf8("textArea"));
    ui->gridLayout->addWidget(textArea, 0, 1, -1, 6);

    QList<Symbol> listOfSymbols;
    *contentStream >> listOfSymbols;
    for(const auto& sym : listOfSymbols){
        textArea->addSymbolToList(sym, "");
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

    QObject::connect(this->fontSelector,  &QFontComboBox::currentFontChanged, this, &MainEditor::selectFont);
    QObject::connect(this->sizeSelector,  &QComboBox::currentTextChanged, this, &MainEditor::selectSize);
    QObject::connect(ui->share, SIGNAL(triggered()), this, SLOT(shareLink()));
}


void MainEditor::updateStyleButton(const QTextCharFormat &f) {
#ifdef DEBUG
    qDebug() << "format has changed";
#endif
    ui->italic->setChecked( f.fontItalic());
    ui->underline->setChecked( f.fontUnderline());
    ui->bold->setChecked( f.fontWeight() == QFont::Bold);
}

void MainEditor::Bold() {
#ifdef DEBUG
    qDebug() << "Ha dittu bold?";
#endif
    QTextCharFormat format = this->textArea->textCursor().charFormat();
    this->textArea->setIsBold(!this->textArea->isBold1());
    format.setFontWeight(this->textArea->isBold1() ? QFont::Bold : QFont::Normal);
    this->textArea->mergeCurrentCharFormat(format);
}

void MainEditor::Italic() {
    QTextCharFormat format = this->textArea->textCursor().charFormat();
    this->textArea->setIsItalic(!this->textArea->isItalic1());
    format.setFontItalic(this->textArea->isItalic1());
    this->textArea->mergeCurrentCharFormat(format);
}

void MainEditor::Underline() {
    QTextCharFormat format = this->textArea->textCursor().charFormat();
    this->textArea->setIsUnderline(!this->textArea->isUnderline1());
    format.setFontUnderline(!this->textArea->isUnderline1());
    this->textArea->mergeCurrentCharFormat(format);
}

void MainEditor::selectFont() {
    QTextCharFormat format;
    format.setFontFamily(this->fontSelector->currentText());
    this->textArea->mergeCurrentCharFormat(format);
}

void MainEditor::selectSize() {
    QTextCharFormat format;
    format.setFontPointSize(this->sizeSelector->currentText().toDouble());
    this->textArea->mergeCurrentCharFormat(format);
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
#if DEBUG
    qDebug() << "Saving file " + filename;
#endif
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

        // TODO: add the username to highlight the insertion
        message["username"] = this->username;

        // add filename to message
        message["filename"] = this->filename;
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        auto messageFormat = QByteArray::fromBase64(message["format"].toString().toLatin1());
        QDataStream inFormatStream(&messageFormat, QIODevice::ReadOnly);
        QTextCharFormat format;
        inFormatStream >> format;

        if (!this->tcpSocket->write(block)) {
            ui->statusBar->showMessage(tr("Could not send message to the server, char will be not inserted."), 5000);
        }
        this->tcpSocket->flush();
#if DEBUG
        qDebug() << "Sending char " << message["unicode"] << " at position " << message["position"]
                 << " with format I:" << format.fontItalic() << "; U:" << format.fontUnderline() << "; B:" << (format.fontWeight() == QFont::Bold);
#endif
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
        message["username"] = this->username;

        // send the JSON using QDataStream
        out << QJsonDocument(message).toJson();

        if (!this->tcpSocket->write(block)) {
            ui->statusBar->showMessage(tr("Could not save the file.\nTry again later."), 5000);
        }
        this->tcpSocket->flush();
#if DEBUG
        qDebug() <<"Sending deletion of" << message["charId"];
#endif
    }
}

void MainEditor::receiveSymbol(QJsonValueRef content, QJsonValueRef username) {
    auto data = QByteArray::fromBase64(content.toString().toLatin1());
    QDataStream inStream(&data, QIODevice::ReadOnly);

    Symbol sym;
    inStream >> sym;

    // TODO: add username highlighting
    QString user = username.toString().toLatin1();
    QString color;
    if (user != this->username)
        color = userMap.value(user);
    else
        color = "";
    this->textArea->addSymbolToList(sym, color);
}

void MainEditor::receiveDeletion(QJsonValueRef id, QJsonValueRef position, QJsonValueRef username) {
    auto symId = id.toString();
    auto symPos = position.toString();

    QString user = username.toString();
    QString color;
    if (user != this->username)
        color = userMap.value(user);
    else
        color = "";

#if DEBUG
    qDebug() << "Received the deletion of char" << symId << "at position" << symPos;
#endif

    this->textArea->removeSymbolFromList(symId, symPos, color);
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
        message["username"] = this->username;

        // send the JSON using QDataStream
        const QByteArray &outputJson = QJsonDocument(message).toJson();
#if DEBUG
        qDebug() << "Sending message of length" << outputJson.size();
#endif
        out << outputJson;

        if (!this->tcpSocket->write(block)) {
            ui->statusBar->showMessage(tr("Could not save the file.\nTry again later."), 5000);
        }
        this->tcpSocket->flush();
    }

}

void MainEditor::receiveBatchDeletion(QJsonValueRef idsAndPositionsJson, QJsonValueRef user) {
    auto idsPositionsBytes = QByteArray::fromBase64(idsAndPositionsJson.toString().toLatin1());
    QDataStream inIdsPositionBytesStream(&idsPositionsBytes, QIODevice::ReadOnly);

    QHash<QString, FracPosition> idsAndPositions;
    inIdsPositionBytesStream >> idsAndPositions;

    QString name = user.toString();
    QString color;
    if (name != this->username)
        color = this->userMap.value(name);
    else
        color = "";

#ifdef DEBUG
    qDebug() << "remove symbol";
#endif
    for(auto key : idsAndPositions.keys()){
        this->textArea->removeSymbolFromList(key, const_cast<QString &>(idsAndPositions.value(key).getStringPosition()), color);
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
        metadata["username"] = this->username;

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
#if DEBUG
    qDebug() << "Going to paste" << arrayLength << "characters";
#endif

    // extract username of user who performed the operation
    QString user = metadata["username"].toString();
    QString color;
    if (user != this->username)
        color = this->userMap.value(user);
    else
        color = "";

    for(int i = 0; i < arrayLength; i++){
        QJsonObject singleSymbol = data[i+1].toObject();
        FracPosition fp(singleSymbol["fracPosition"].toString());
        QChar unicode = singleSymbol["unicode"].toString()[0];
        QString charId = singleSymbol["charId"].toString();
        QTextCharFormat format = formats[0];

        Symbol sym(unicode, charId, fp, format);
#if DEBUG
        qDebug() << "Batch inserting" << sym.getCharacter() << "at position" << sym.getPosition().getStringPosition();
#endif
        this->textArea->addSymbolToList(sym, color);
    }
}

void MainEditor::colors(QString username, QString color){
#if DEBUG
    qDebug() << "User: " << username << " has color: " << color;
#endif

    if (this->userMap.isEmpty()){
        // received the first user -> delete the first item
        this->onlineUsers->takeItem(0);
    }

    // check if user already online
    if (!this->userMap.contains(username)){
        // user was not online
        this->userMap.insert(username, color);

        // insert the user with its color
        QColor userColor{color};
        this->onlineUsers->addItem(username);
        onlineUsers->item(this->userMap.size()-1)->setForeground(userColor);
    } else if (color == "-1"){
        // user has logged out
        this->userMap.remove(username);
        for (QListWidgetItem *item : this->onlineUsers->findItems(username, Qt::MatchExactly))
            this->onlineUsers->takeItem(this->onlineUsers->row(item));
    }
}

void MainEditor::exportAsPDF() {
    QFileDialog fileDialog(this, tr("Export as PDF"));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setMimeTypeFilters(QStringList("application/pdf"));
    fileDialog.setDefaultSuffix("pdf");
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    QString fileName = fileDialog.selectedFiles().first();
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    this->textArea->document()->print(&printer);
    statusBar()->showMessage(tr("Exported \"%1\"").arg(QDir::toNativeSeparators(fileName)));
}

void MainEditor::shareLink() {
    QString link = "PDS-SharedEditor/" + this->filename;
    QMessageBox shareDialog{QMessageBox::Information, "Share Link", "Share this link to collaborators", QMessageBox::NoButton, this};
    shareDialog.setInformativeText(link);
    shareDialog.setDetailedText("Anyone who accesses the file through this link can view and modify the file");
    shareDialog.setStandardButtons(QMessageBox::Ok);
    shareDialog.setDefaultButton(QMessageBox::Ok);
    shareDialog.exec();
}

void MainEditor::sendCharFormatChanged(QJsonObject message) {
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
        message["username"] = this->username;

        // send the JSON using QDataStream
        const QByteArray &outputJson = QJsonDocument(message).toJson();
#if DEBUG
        qDebug() << "Sending message of length" << outputJson.size();
#endif
        out << outputJson;

        if (!this->tcpSocket->write(block)) {
            ui->statusBar->showMessage(tr("Could not save the file.\nTry again later."), 5000);
        }
        this->tcpSocket->flush();
    }

}

void MainEditor::receiveFormatChanged(QJsonValueRef idsAndPositions, QJsonValueRef format) {
#ifdef DEBUG
    qDebug() << "Io provo a cambiare il formato";
#endif
    auto idsPositionsBytes = QByteArray::fromBase64(idsAndPositions.toString().toLatin1());
    auto formatBytes = QByteArray::fromBase64(format.toString().toLatin1());
    QDataStream inIdsPositionBytesStream(&idsPositionsBytes, QIODevice::ReadOnly);
    QDataStream inFormatStream(&formatBytes, QIODevice::ReadOnly);

    QHash<QString, FracPosition> idsAndPositionsHash;
    QTextCharFormat textCharFormat;
    inIdsPositionBytesStream >> idsAndPositionsHash;
    inFormatStream >> textCharFormat;
#ifdef  DEBUG
    qDebug() << "in MainEditor, font weight is" << textCharFormat.fontWeight();
#endif

    for(auto key : idsAndPositionsHash.keys()){
        this->textArea->changeSymbolFormat(key, const_cast<QString &>(idsAndPositionsHash.value(key).getStringPosition()), textCharFormat);
    }

}




