#ifndef EDITOR_TEXTEDIT_H
#define EDITOR_TEXTEDIT_H

#include <QMainWindow>
#include <QMap>
#include <QPointer>

QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QFontComboBox;
class QTextEdit;
class QTextCharFormat;
class QMenu;
class QPrinter;
QT_END_NAMESPACE

class TextEdit: public QMainWindow {
	Q_OBJECT
	friend class LoginWindow;

private:
	QAction* actionSave;
	QAction* actionTextBold;
	QAction* actionTextUnderline;
	QAction* actionTextItalic;
	QAction* actionTextColor;
	QAction* actionAlignLeft;
	QAction* actionAlignCenter;
	QAction* actionAlignRight;
	QAction* actionAlignJustify;
	QAction* actionUndo;
	QAction* actionRedo;
#ifndef QT_NO_CLIPBOARD
	QAction* actionCut;
	QAction* actionCopy;
	QAction* actionPaste;
#endif
	QComboBox* comboStyle;
	QFontComboBox* comboFont;
	QComboBox* comboSize;
	QToolBar* tb;
	QString fileName;
	QTextEdit* textEdit;
	
	// campi aggiunti da noi
	QString userName;
	
	void setupFileActions();
	void setupEditActions();
	void setupTextActions();
	bool maybeSave();
	void setCurrentFileName(const QString& fileName);
	void mergeFormatOnWordOrSelection(const QTextCharFormat& format);
	void fontChanged(const QFont& f);
	void colorChanged(const QColor& c);
	void alignmentChanged(Qt::Alignment a);
	
	// metodi aggiunti da noi;
	void readSettings();
	void setUsername(QString user);
	void welcomeStatusBar();

protected:
	void closeEvent(QCloseEvent* e) override;

public:
	explicit TextEdit(QWidget* parent = 0);
	bool load(const QString& f);
	
private slots:
	void fileOpen();
	bool fileSave();
	bool fileSaveAs();
	void filePrint();
	void filePrintPreview();
	void filePrintPdf();
	void textBold();
	void textUnderline();
	void textItalic();
	void textFamily(const QString& f);
	void textSize(const QString& p);
	void textStyle(int styleIndex);
	void textColor();
	void textAlign(QAction* a);
	void currentCharFormatChanged(const QTextCharFormat& format);
	void cursorPositionChanged();
	void clipboardDataChanged();
	void about();
	void printPreview(QPrinter* printer);
	
public slots:
	void fileNew();
};

#endif // EDITOR_TEXTEDIT_H