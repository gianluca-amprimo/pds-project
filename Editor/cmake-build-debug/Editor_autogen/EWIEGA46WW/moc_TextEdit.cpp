/****************************************************************************
** Meta object code from reading C++ file 'TextEdit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../TextEdit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TextEdit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TextEdit_t {
    QByteArrayData data[31];
    char stringdata0[325];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TextEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TextEdit_t qt_meta_stringdata_TextEdit = {
    {
QT_MOC_LITERAL(0, 0, 8), // "TextEdit"
QT_MOC_LITERAL(1, 9, 8), // "fileOpen"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 8), // "fileSave"
QT_MOC_LITERAL(4, 28, 10), // "fileSaveAs"
QT_MOC_LITERAL(5, 39, 9), // "filePrint"
QT_MOC_LITERAL(6, 49, 16), // "filePrintPreview"
QT_MOC_LITERAL(7, 66, 12), // "filePrintPdf"
QT_MOC_LITERAL(8, 79, 8), // "textBold"
QT_MOC_LITERAL(9, 88, 13), // "textUnderline"
QT_MOC_LITERAL(10, 102, 10), // "textItalic"
QT_MOC_LITERAL(11, 113, 10), // "textFamily"
QT_MOC_LITERAL(12, 124, 1), // "f"
QT_MOC_LITERAL(13, 126, 8), // "textSize"
QT_MOC_LITERAL(14, 135, 1), // "p"
QT_MOC_LITERAL(15, 137, 9), // "textStyle"
QT_MOC_LITERAL(16, 147, 10), // "styleIndex"
QT_MOC_LITERAL(17, 158, 9), // "textColor"
QT_MOC_LITERAL(18, 168, 9), // "textAlign"
QT_MOC_LITERAL(19, 178, 8), // "QAction*"
QT_MOC_LITERAL(20, 187, 1), // "a"
QT_MOC_LITERAL(21, 189, 24), // "currentCharFormatChanged"
QT_MOC_LITERAL(22, 214, 15), // "QTextCharFormat"
QT_MOC_LITERAL(23, 230, 6), // "format"
QT_MOC_LITERAL(24, 237, 21), // "cursorPositionChanged"
QT_MOC_LITERAL(25, 259, 20), // "clipboardDataChanged"
QT_MOC_LITERAL(26, 280, 5), // "about"
QT_MOC_LITERAL(27, 286, 12), // "printPreview"
QT_MOC_LITERAL(28, 299, 9), // "QPrinter*"
QT_MOC_LITERAL(29, 309, 7), // "printer"
QT_MOC_LITERAL(30, 317, 7) // "fileNew"

    },
    "TextEdit\0fileOpen\0\0fileSave\0fileSaveAs\0"
    "filePrint\0filePrintPreview\0filePrintPdf\0"
    "textBold\0textUnderline\0textItalic\0"
    "textFamily\0f\0textSize\0p\0textStyle\0"
    "styleIndex\0textColor\0textAlign\0QAction*\0"
    "a\0currentCharFormatChanged\0QTextCharFormat\0"
    "format\0cursorPositionChanged\0"
    "clipboardDataChanged\0about\0printPreview\0"
    "QPrinter*\0printer\0fileNew"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TextEdit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  114,    2, 0x08 /* Private */,
       3,    0,  115,    2, 0x08 /* Private */,
       4,    0,  116,    2, 0x08 /* Private */,
       5,    0,  117,    2, 0x08 /* Private */,
       6,    0,  118,    2, 0x08 /* Private */,
       7,    0,  119,    2, 0x08 /* Private */,
       8,    0,  120,    2, 0x08 /* Private */,
       9,    0,  121,    2, 0x08 /* Private */,
      10,    0,  122,    2, 0x08 /* Private */,
      11,    1,  123,    2, 0x08 /* Private */,
      13,    1,  126,    2, 0x08 /* Private */,
      15,    1,  129,    2, 0x08 /* Private */,
      17,    0,  132,    2, 0x08 /* Private */,
      18,    1,  133,    2, 0x08 /* Private */,
      21,    1,  136,    2, 0x08 /* Private */,
      24,    0,  139,    2, 0x08 /* Private */,
      25,    0,  140,    2, 0x08 /* Private */,
      26,    0,  141,    2, 0x08 /* Private */,
      27,    1,  142,    2, 0x08 /* Private */,
      30,    0,  145,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 28,   29,
    QMetaType::Void,

       0        // eod
};

void TextEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TextEdit *_t = static_cast<TextEdit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fileOpen(); break;
        case 1: { bool _r = _t->fileSave();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 2: { bool _r = _t->fileSaveAs();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->filePrint(); break;
        case 4: _t->filePrintPreview(); break;
        case 5: _t->filePrintPdf(); break;
        case 6: _t->textBold(); break;
        case 7: _t->textUnderline(); break;
        case 8: _t->textItalic(); break;
        case 9: _t->textFamily((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->textSize((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->textStyle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->textColor(); break;
        case 13: _t->textAlign((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 14: _t->currentCharFormatChanged((*reinterpret_cast< const QTextCharFormat(*)>(_a[1]))); break;
        case 15: _t->cursorPositionChanged(); break;
        case 16: _t->clipboardDataChanged(); break;
        case 17: _t->about(); break;
        case 18: _t->printPreview((*reinterpret_cast< QPrinter*(*)>(_a[1]))); break;
        case 19: _t->fileNew(); break;
        default: ;
        }
    }
}

const QMetaObject TextEdit::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_TextEdit.data,
      qt_meta_data_TextEdit,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TextEdit.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int TextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 20;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
