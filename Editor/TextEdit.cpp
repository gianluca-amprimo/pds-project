#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QTextCodec>
#include <QTextEdit>
#include <QStatusBar>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QMimeData>
#include <QtCore>
#include <QDesktopWidget>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif
#include <QPrinter>
#if QT_CONFIG(printpreviewdialog)
#include <QPrintPreviewDialog>
#endif
#endif
#endif

#include "TextEdit.h"

#ifdef Q_OS_MAC
const QString rsrcPath = ":/images/mac";
#else
const QString rsrcPath = ":/images/win";
#endif

TextEdit::TextEdit(QWidget* parent): QMainWindow(parent) {
#ifdef Q_OS_OSX
	setUnifiedTitleAndToolBarOnMac(true);
#endif
	setWindowTitle(QCoreApplication::applicationName());                // imposta il nome della finestra
	
	textEdit = new QTextEdit(this);                             // inizializza l'editor
	connect(textEdit, &QTextEdit::currentCharFormatChanged, this, &TextEdit::currentCharFormatChanged);
	connect(textEdit, &QTextEdit::cursorPositionChanged, this, &TextEdit::cursorPositionChanged);
	setCentralWidget(textEdit);
	
	setToolButtonStyle(Qt::ToolButtonFollowStyle);
	setupFileActions();
	setupEditActions();
	setupTextActions();
	
	{
		auto helpMenu = menuBar()->addMenu(("Help"));
		helpMenu->addAction(tr("About"), this, &TextEdit::about);
		helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
	}
	
	QFont textFont("Helvetica");
	textFont.setStyleHint(QFont::SansSerif);
	textEdit->setFont(textFont);
	fontChanged(textEdit->font());
	colorChanged(textEdit->textColor());
	alignmentChanged(textEdit->alignment());
	
	connect(textEdit->document(), &QTextDocument::modificationChanged, actionSave, &QAction::setEnabled);
	connect(textEdit->document(), &QTextDocument::modificationChanged, this, &QWidget::setWindowModified);
	connect(textEdit->document(), &QTextDocument::undoAvailable, actionUndo, &QAction::setEnabled);
	connect(textEdit->document(), &QTextDocument::redoAvailable, actionRedo, &QAction::setEnabled);
	
	setWindowModified(textEdit->document()->isModified());
	actionSave->setEnabled(textEdit->document()->isModified());
	actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
	actionRedo->setEnabled(textEdit->document()->isRedoAvailable());

#ifndef QT_NO_CLIPBOARD
	actionCut->setEnabled(false);
	connect(textEdit, &QTextEdit::copyAvailable, actionCut, &QAction::setEnabled);
	actionCopy->setEnabled(false);
	connect(textEdit, &QTextEdit::copyAvailable, actionCopy, &QAction::setEnabled);
	connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &TextEdit::clipboardDataChanged);
#endif
	
	textEdit->setFocus();
	setCurrentFileName(QString());

#ifdef Q_OS_MACOS
	// Use dark text on light background on macOS, also in dark mode.
	QPalette pal = textEdit->palette();
	pal.setColor((QPalette::Base, QColor(Qt::white)));
	pal.setColor((QPalette::Text, QColor(Qt::black)));
	textEdit->setPalette(pal);
#endif
	
	readSettings();
}

void TextEdit::closeEvent(QCloseEvent* e) {
	if (maybeSave()) {
		e->accept();
	} else {
		e->ignore();
	}
}

void TextEdit::setupFileActions() {
	QMenu* menu = menuBar()->addMenu(tr("&File"));
	QToolBar* tb = addToolBar(tr("File Actions"));
	
	const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(rsrcPath + "/filenew.png"));
	auto a = menu->addAction(newIcon, tr("&New"), this, &TextEdit::fileNew);
	tb->addAction(a);
	a->setPriority(QAction::LowPriority);
	a->setShortcut(QKeySequence::New);
	
	const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(rsrcPath + "/fileopen.png"));
	a = menu->addAction(openIcon, tr("&Open..."), this, &TextEdit::fileOpen);
	tb->addAction(a);
	a->setShortcut(QKeySequence::Open);
	
	menu->addSeparator();
	
	const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(rsrcPath + "/filesave.png"));
	actionSave = menu->addAction(saveIcon, tr("&Save"), this, &TextEdit::fileSave);
	tb->addAction(actionSave);
	actionSave->setShortcut(QKeySequence::Save);
	actionSave->setEnabled(false);
	
	const QIcon saveAsIcon = QIcon::fromTheme("document-save-as", QIcon(rsrcPath + "/filesaveas.png"));
	a = menu->addAction(saveAsIcon, tr("Save &As..."), this, &TextEdit::fileSaveAs);
	a->setPriority(QAction::LowPriority);
	a->setShortcut(QKeySequence::SaveAs);
	
	menu->addSeparator();

#ifndef QT_NO_PRINTER
	const QIcon printIcon = QIcon::fromTheme("document-print", QIcon(rsrcPath + "/fileprint.png"));
	a = menu->addAction(printIcon, tr("&Print..."), this, &TextEdit::filePrint);
	tb->addAction(a);
	a->setPriority(QAction::LowPriority);
	a->setShortcut(QKeySequence::Print);
	
	const QIcon filePrintIcon = QIcon::fromTheme("fileprint", QIcon(rsrcPath + "/fileprint.png"));
	menu->addAction(filePrintIcon, tr("Print Preview..."), this, &TextEdit::filePrintPreview);
	
	const QIcon exportPdfIcon = QIcon::fromTheme("exportpdf", QIcon(rsrcPath + "/exportpdf.png"));
	a = menu->addAction(exportPdfIcon, tr("&Export PDF..."), this, &TextEdit::filePrintPdf);
	tb->addAction(a);
	a->setPriority(QAction::LowPriority);
	a->setShortcut(Qt::CTRL + Qt::Key_D);
	
	menu->addSeparator();
#endif

	a = menu->addAction(tr("&Quit"), this, &QWidget::close);
	a->setShortcut(Qt::CTRL + Qt::Key_Q);
}

void TextEdit::setupEditActions() {
	QMenu* menu = menuBar()->addMenu(tr("&Edit"));
	QToolBar* tb = addToolBar(tr("Edit Actions"));
	
	const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(rsrcPath + "/editundo.png"));
	actionUndo = menu->addAction(undoIcon, tr("&Undo"), textEdit, &QTextEdit::undo);
	tb->addAction(actionUndo);
	actionUndo->setPriority(QAction::LowPriority);
	actionUndo->setShortcut(QKeySequence::Undo);
	
	const QIcon redoIcon = QIcon::fromTheme("edit-redo", QIcon(rsrcPath + "/editredo.png"));
	actionRedo = menu->addAction(redoIcon, tr("&Redo"), textEdit, &QTextEdit::redo);
	tb->addAction(actionRedo);
	actionRedo->setPriority(QAction::LowPriority);
	actionRedo->setShortcut(QKeySequence::Redo);
	
	menu->addSeparator();

#ifndef QT_NO_CLIPBOARD
	const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(rsrcPath + "/editcut.png"));
	actionCut = menu->addAction(cutIcon, tr("Cu&t"), textEdit, &QTextEdit::cut);
	tb->addAction(actionCut);
	actionCut->setPriority(QAction::LowPriority);
	actionCut->setShortcut(QKeySequence::Cut);
	
	const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(rsrcPath + "/editcopy.png"));
	actionCopy = menu->addAction(copyIcon, tr("&Copy"), textEdit, &QTextEdit::copy);
	tb->addAction(actionCopy);
	actionCopy->setPriority(QAction::LowPriority);
	actionCopy->setShortcut(QKeySequence::Copy);
	
	const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(rsrcPath + "/editpaste.png"));
	actionPaste = menu->addAction(pasteIcon, tr("&Paste"), textEdit, &QTextEdit::paste);
	tb->addAction(actionPaste);
	actionPaste->setPriority(QAction::LowPriority);
	actionPaste->setShortcut(QKeySequence::Paste);
	if (const QMimeData* md = QApplication::clipboard()->mimeData()) {
		actionPaste->setEnabled(md->hasText());
	}
#endif
}

void TextEdit::setupTextActions() {
	QMenu* menu = menuBar()->addMenu(tr("F&ormat"));
	QToolBar* tb = addToolBar(tr("Format Actions"));
	
	const QIcon boldIcon = QIcon::fromTheme("format-text-bold", QIcon(rsrcPath + "/textbold.png"));
	actionTextBold = menu->addAction(boldIcon, tr("&Bold"), this, &TextEdit::textBold);
	tb->addAction(actionTextBold);
	actionTextBold->setPriority(QAction::LowPriority);
	actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
	QFont bold;
	bold.setBold(true);
	actionTextBold->setFont(bold);
	actionTextBold->setCheckable(true);
	
	const QIcon italicIcon = QIcon::fromTheme("format-text-italic", QIcon(rsrcPath + "/textitalic.png"));
	actionTextItalic = menu->addAction(italicIcon, tr("&Italic"), this, &TextEdit::textItalic);
	tb->addAction(actionTextItalic);
	actionTextItalic->setPriority(QAction::LowPriority);
	actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
	QFont italic;
	italic.setItalic(true);
	actionTextItalic->setFont(italic);
	actionTextItalic->setCheckable(true);
	
	const QIcon underlineIcon = QIcon::fromTheme("format-text-underline", QIcon(rsrcPath + "/textunder.png"));
	actionTextUnderline = menu->addAction(underlineIcon, tr("&Underline"), this, &TextEdit::textUnderline);
	tb->addAction(actionTextUnderline);
	actionTextUnderline->setPriority(QAction::LowPriority);
	actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
	QFont underline;
	underline.setUnderline(true);
	actionTextUnderline->setFont(underline);
	actionTextUnderline->setCheckable(true);
	
	menu->addSeparator();
	
	const QIcon leftIcon = QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png"));
	actionAlignLeft = new QAction(leftIcon, tr("&Left"), this);
	actionAlignLeft->setPriority(QAction::LowPriority);
	actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
	actionAlignLeft->setCheckable(true);
	
	const QIcon centerIcon = QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png"));
	actionAlignCenter = new QAction(centerIcon, tr("C&enter"), this);
	actionAlignCenter->setPriority(QAction::LowPriority);
	actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
	actionAlignCenter->setCheckable(true);
	
	const QIcon rightIcon = QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png"));
	actionAlignRight = new QAction(rightIcon, tr("&Right"), this);
	actionAlignRight->setPriority(QAction::LowPriority);
	actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
	actionAlignRight->setCheckable(true);
	
	const QIcon fillIcon = QIcon::fromTheme("format-justify-fill", QIcon(rsrcPath + "/textjustify.png"));
	actionAlignJustify = new QAction(fillIcon, tr("&Justify"), this);
	actionAlignJustify->setPriority(QAction::LowPriority);
	actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
	actionAlignJustify->setCheckable(true);
	
	// Make sure the alignLeft is always left of the alignRight
	auto alignGroup = new QActionGroup(this);
	connect(alignGroup, &QActionGroup::triggered, this, &TextEdit::textAlign);
	
	if (QApplication::isLeftToRight()) {
		alignGroup->addAction(actionAlignLeft);
		alignGroup->addAction(actionAlignCenter);
		alignGroup->addAction(actionAlignRight);
	} else {
		alignGroup->addAction(actionAlignRight);
		alignGroup->addAction(actionAlignCenter);
		alignGroup->addAction(actionAlignLeft);
	}
	alignGroup->addAction(actionAlignJustify);
	
	menu->addActions(alignGroup->actions());
	tb->addActions(alignGroup->actions());
	
	menu->addSeparator();
	
	QPixmap pix(16, 16);
	pix.fill(Qt::black);
	actionTextColor = menu->addAction(pix, tr("&Color"), this, &TextEdit::textColor);
	tb->addAction(actionTextColor);
	
	tb = addToolBar(tr("Format Actions"));
	tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
	addToolBarBreak(Qt::TopToolBarArea);
	addToolBar(tb);
	
	comboStyle = new QComboBox(tb);
	tb->addWidget(comboStyle);
	comboStyle->addItem("Standard");
	comboStyle->addItem("Bullet List (Disc)");
	comboStyle->addItem("Bullet List (Circle)");
	comboStyle->addItem("Bullet List (Square)");
	comboStyle->addItem("Ordered List (Decimal)");
	comboStyle->addItem("Ordered List (Alpha lower)");
	comboStyle->addItem("Ordered List (Alpha upper)");
	comboStyle->addItem("Ordered List (Roman lower)");
	comboStyle->addItem("Ordered List (Roman upper)");
	comboStyle->addItem("Heading 1");
	comboStyle->addItem("Heading 2");
	comboStyle->addItem("Heading 3");
	comboStyle->addItem("Heading 4");
	comboStyle->addItem("Heading 5");
	comboStyle->addItem("Heading 6");
	
	connect(comboStyle, QOverload<int>::of(&QComboBox::activated), this, &TextEdit::textStyle);
	
	comboFont = new QFontComboBox(tb);
	tb->addWidget(comboFont);
	connect(comboFont, QOverload<const QString&>::of(&QComboBox::activated), this, &TextEdit::textFamily);
	
	comboSize = new QComboBox(tb);
	comboSize->setObjectName("comboSize");
	tb->addWidget(comboSize);
	comboSize->setEditable(true);
	const QList<int> standardSizes = QFontDatabase::standardSizes();
	foreach (int size, standardSizes) {
		comboSize->addItem(QString::number(size));
	}
	comboSize->setCurrentIndex(standardSizes.indexOf(QApplication::font().pointSize()));
	connect(comboSize, QOverload<const QString&>::of(&QComboBox::activated), this, &TextEdit::textSize);
}

bool TextEdit::load(const QString& f) {
	if (!QFile::exists(f)) {
		return false;
	}
	
	QFile file(f);
	if (!file.open(QFile::ReadOnly)) {
		return false;
	}
	
	QByteArray data = file.readAll();
	QTextCodec* codec = Qt::codecForHtml(data);
	QString str = codec->toUnicode(data);
	if (Qt::mightBeRichText(str)) {
		textEdit->setHtml(str);
	} else {
		str = QString::fromLocal8Bit(data);
		textEdit->setPlainText(str);
	}
	
	setCurrentFileName(f);
	return true;
}

bool TextEdit::maybeSave() {
	if (!textEdit->document()->isModified()) {
		return true;
	}
	
	const QMessageBox::StandardButton ret = QMessageBox::warning(this, QCoreApplication::applicationName(), tr("The document has been modified.\nDo you want to save your changes?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	if (ret == QMessageBox::Save) {
		return fileSave();
	} else if (ret == QMessageBox::Cancel) {
		return false;
	}
	return true;
}

void TextEdit::setCurrentFileName(const QString& fileName) {
	this->fileName = fileName;
	textEdit->document()->setModified(false);
	
	QString shownName;
	if (fileName.isEmpty()) {
		shownName = "untitled.txt";
	} else {
		shownName = QFileInfo(fileName).fileName();
	}
	
	setWindowTitle(tr("%1[*] - %2").arg(shownName, QCoreApplication::applicationName()));
	setWindowModified(false);
}

void TextEdit::fileNew() {
	if (maybeSave()) {
		textEdit->clear();
		setCurrentFileName(QString());
	}
}

void TextEdit::fileOpen() {
	QFileDialog fileDialog(this, tr("Open File..."));
	fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
	fileDialog.setFileMode(QFileDialog::ExistingFile);
	fileDialog.setMimeTypeFilters(QStringList() << "text/html" << "text/plain");
	if (fileDialog.exec() != QDialog::Accepted) {
		return;
	}
	const QString fn = fileDialog.selectedFiles().first();
	if (load(fn)) {
		statusBar()->showMessage(tr("Opened \"%1\"").arg(QDir::toNativeSeparators(fn)));
	} else {
		statusBar()->showMessage(tr("Could not open \"%1\"").arg(QDir::toNativeSeparators(fn)));
	}
}

bool TextEdit::fileSave() {
	if (fileName.isEmpty()) {
		return fileSaveAs();
	}
	
	if (fileName.startsWith(QString(":/"))) {
		return fileSaveAs();
	}
	
	QTextDocumentWriter writer(fileName);
	bool success = writer.write(textEdit->document());
	if (success) {
		textEdit->document()->setModified(false);
		statusBar()->showMessage(tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName)));
	} else {
		statusBar()->showMessage(tr("Could not write to file \"%1\"").arg(QDir::toNativeSeparators(fileName)));
	}
	
	return success;
}

bool TextEdit::fileSaveAs() {
	QFileDialog fileDialog(this, tr("Save as..."));
	fileDialog.setAcceptMode(QFileDialog::AcceptSave);
	QStringList mimeTypes;
	mimeTypes << "application/vnd.oasis.opendocument.text" << "text/html" << "text/plain";
	fileDialog.setMimeTypeFilters(mimeTypes);
	fileDialog.setDefaultSuffix("odt");
	if (fileDialog.exec() != QDialog::Accepted) {
		return false;
	}
	const QString fn = fileDialog.selectedFiles().first();
	setCurrentFileName(fn);
	return fileSave();
}

void TextEdit::filePrint() {
#if QT_CONFIG(printdialog)
	QPrinter printer(QPrinter::HighResolution);
	auto dlg = new  QPrintDialog(&printer, this);
	if (textEdit->textCursor().hasSelection()) {
		dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
	}
	dlg->setWindowTitle(tr("Print Document"));
	if (dlg->exec() == QDialog::Accepted) {
		textEdit->print(&printer);
	}
	delete dlg;
#endif
}

void TextEdit::filePrintPreview() {
#if QT_CONFIG(printpreviewdialog)
	QPrinter printer(QPrinter::HighResolution);
	QPrintPreviewDialog preview(&printer, this);
	connect(&preview, &QPrintPreviewDialog::paintRequested, this, &TextEdit::printPreview);
	preview.exec();
#endif
}

void TextEdit::printPreview(QPrinter* printer) {
#ifdef  QT_NO_PRINTER
	Q_UNUSED(printer);
#else
	textEdit->print(printer);
#endif
}

void TextEdit::filePrintPdf() {
#ifndef QT_NO_PRINTER
	QFileDialog fileDialog(this, tr("Export PDF"));
	fileDialog.setAcceptMode(QFileDialog::AcceptSave);
	fileDialog.setMimeTypeFilters(QStringList("application/pdf"));
	fileDialog.setDefaultSuffix("pdf");
	if (fileDialog.exec() != QDialog::Accepted) {
		return;
	}
	QString fileName = fileDialog.selectedFiles().first();
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fileName);
	textEdit->document()->print(&printer);
	statusBar()->showMessage(tr("Exported \"%1\"").arg(QDir::toNativeSeparators(fileName)));
#endif
}

void TextEdit::textBold() {
	QTextCharFormat fmt;
	if (actionTextBold->isChecked()) {
		fmt.setFontWeight(QFont::Bold);
	} else {
		fmt.setFontWeight(QFont::Normal);
	}
	mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textUnderline() {
	QTextCharFormat fmt;
	fmt.setFontUnderline(actionTextUnderline->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textItalic() {
	QTextCharFormat fmt;
	fmt.setFontItalic(actionTextItalic->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textFamily(const QString& f) {
	QTextCharFormat fmt;
	fmt.setFontFamily(f);
	mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textSize(const QString& p) {
	qreal pointSize = p.toFloat();
	if (p.toFloat() > 0) {
		QTextCharFormat fmt;
		fmt.setFontPointSize(pointSize);
		mergeFormatOnWordOrSelection(fmt);
	}
}

void TextEdit::textStyle(int styleIndex) {
	QTextCursor cursor = textEdit->textCursor();
	QTextListFormat::Style style = QTextListFormat::ListStyleUndefined;
	
	if (styleIndex == 1) {
		style = QTextListFormat::ListDisc;
	} else if (styleIndex == 2) {
		style = QTextListFormat::ListCircle;
	} else if (styleIndex == 3) {
		style = QTextListFormat::ListSquare;
	} else if (styleIndex == 4) {
		style = QTextListFormat::ListDecimal;
	} else if (styleIndex == 5) {
		style = QTextListFormat::ListLowerAlpha;
	} else if (styleIndex == 6) {
		style = QTextListFormat::ListUpperAlpha;
	} else if (styleIndex == 7) {
		style = QTextListFormat::ListLowerRoman;
	} else if (styleIndex == 8) {
		style = QTextListFormat::ListUpperRoman;
	}
	
	cursor.beginEditBlock();
	
	QTextBlockFormat blockFmt = cursor.blockFormat();
	
	if (style == QTextListFormat::ListStyleUndefined) {
		blockFmt.setObjectIndex(-1);
		int headingLevel;
		if (styleIndex >= 9) {
			headingLevel = styleIndex - 9 + 1;
		} else {
			headingLevel = 0;
		}
//		blockFmt.setHeadingLevel(headingLevel);         // NON SUPPORTATO IN QUESTA VERSIONE DI QT
		cursor.setBlockFormat(blockFmt);
		
		int sizeAdjustment;
		if (headingLevel) {
			sizeAdjustment = 4 - headingLevel;
		} else {
			sizeAdjustment = 0;
		}
		QTextCharFormat fmt;
		if (headingLevel) {
			fmt.setFontWeight(QFont::Bold);
		} else {
			fmt.setFontWeight(QFont::Normal);
		}
		fmt.setProperty(QTextFormat::FontSizeAdjustment, sizeAdjustment);
		cursor.select(QTextCursor::LineUnderCursor);
		cursor.mergeCharFormat(fmt);
		textEdit->mergeCurrentCharFormat(fmt);
	} else {
		QTextListFormat listFmt;
		if (cursor.currentList()) {
			listFmt = cursor.currentList()->format();
		} else {
			listFmt.setIndent(blockFmt.indent() + 1);
			blockFmt.setIndent(0);
			cursor.setBlockFormat(blockFmt);
		}
		listFmt.setStyle(style);
		cursor.createList(listFmt);
	}
	cursor.endEditBlock();
}

void TextEdit::textColor() {
	QColor col = QColorDialog::getColor(textEdit->textColor(), this);
	if (!col.isValid()) {
		return;
	}
	QTextCharFormat fmt;
	fmt.setForeground(col);
	mergeFormatOnWordOrSelection(fmt);
	colorChanged(col);
}

void TextEdit::textAlign(QAction* a) {
	if (a == actionAlignLeft) {
		textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
	} else if (a == actionAlignCenter) {
		textEdit->setAlignment(Qt::AlignHCenter);
	} else if (a == actionAlignRight) {
		textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
	} else if (a == actionAlignJustify) {
		textEdit->setAlignment(Qt::AlignJustify);
	}
}

void TextEdit::currentCharFormatChanged(const QTextCharFormat& format) {
	fontChanged(format.font());
	colorChanged(format.foreground().color());
}

void TextEdit::cursorPositionChanged() {
	alignmentChanged(textEdit->alignment());
	QTextList* list = textEdit->textCursor().currentList();
	if (list) {
		if (list->format().style() == QTextListFormat::ListDisc) {
			comboStyle->setCurrentIndex(1);
		} else if (list->format().style() == QTextListFormat::ListCircle) {
			comboStyle->setCurrentIndex(2);
		} else if (list->format().style() == QTextListFormat::ListSquare) {
			comboStyle->setCurrentIndex(3);
		} else if (list->format().style() == QTextListFormat::ListDecimal) {
			comboStyle->setCurrentIndex(4);
		} else if (list->format().style() == QTextListFormat::ListLowerAlpha) {
			comboStyle->setCurrentIndex(5);
		} else if (list->format().style() == QTextListFormat::ListUpperAlpha) {
			comboStyle->setCurrentIndex(6);
		} else if (list->format().style() == QTextListFormat::ListLowerRoman) {
			comboStyle->setCurrentIndex(7);
		} else if (list->format().style() == QTextListFormat::ListUpperRoman) {
			comboStyle->setCurrentIndex(8);
		} else {
			comboStyle->setCurrentIndex(-1);
		}
	} else {
//		int headingLevel = textEdit->textCursor().blockFormat().headingLevel();         // NON SUPPORTATO IN QUESTA VERSIONE DI QT
//		comboStyle->setCurrentIndex(headingLevel ? headingLevel + 8 : 0);
		comboStyle->setCurrentIndex(-1);
	}
}

void TextEdit::clipboardDataChanged() {
#ifndef QT_NO_CLIPBOARD
	if (const QMimeData* md = QApplication::clipboard()->mimeData()) {
		actionPaste->setEnabled(md->hasText());
	}
#endif
}

void TextEdit::about() {
	QMessageBox::about(this, tr("About"), tr("This is a collaborative editor programmed using QT libraries"));
}

void TextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format) {
	QTextCursor cursor = textEdit->textCursor();
	if (!cursor.hasSelection()) {
		cursor.select(QTextCursor::WordUnderCursor);
	}
	cursor.mergeCharFormat(format);
	textEdit->mergeCurrentCharFormat(format);
}

void TextEdit::fontChanged(const QFont& f) {
	comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
	comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
	actionTextBold->setChecked(f.bold());
	actionTextItalic->setChecked(f.italic());
	actionTextUnderline->setChecked(f.underline());
}

void TextEdit::colorChanged(const QColor& c) {
	QPixmap pix(16, 16);
	pix.fill(c);
	actionTextColor->setIcon(pix);
}

void TextEdit::alignmentChanged(Qt::Alignment a) {
	if (a & Qt::AlignLeft) {
		actionAlignLeft->setChecked(true);
	} else if (a & Qt::AlignHCenter) {
		actionAlignCenter->setChecked(true);
	} else if (a & Qt::AlignRight) {
		actionAlignRight->setChecked(true);
	} else if (a & Qt::AlignJustify) {
		actionAlignJustify->setChecked(true);
	}
}

//////////////////////////////////////////////////////////////////
//                   METODI AGGIUNTI DA NOI                   //
//////////////////////////////////////////////////////////////////

void TextEdit::readSettings() {
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
	/*
	if (geometry.isEmpty()) {
		const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
		resize(availableGeometry.width() / 1.5, availableGeometry.height() / 1.5);
		move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
	} else {
		restoreGeometry(geometry);
	}
	*/
	const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
	resize(availableGeometry.width() / 1.5, availableGeometry.height() / 1.5);
	move((availableGeometry.width() - width()) / 2,(availableGeometry.height() - height()) / 2);
}

void TextEdit::setUsername(QString user) {
	userName = user;
}

void TextEdit::welcomeStatusBar() {
	statusBar()->showMessage(tr("Welcome online, %1.").arg(userName));
}