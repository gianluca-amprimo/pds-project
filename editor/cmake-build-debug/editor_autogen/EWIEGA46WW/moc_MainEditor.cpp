/****************************************************************************
** Meta object code from reading C++ file 'MainEditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../MainEditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainEditor_t {
    QByteArrayData data[14];
    char stringdata0[129];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainEditor_t qt_meta_stringdata_MainEditor = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainEditor"
QT_MOC_LITERAL(1, 11, 4), // "Bold"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 6), // "Italic"
QT_MOC_LITERAL(4, 24, 9), // "Underline"
QT_MOC_LITERAL(5, 34, 10), // "selectFont"
QT_MOC_LITERAL(6, 45, 4), // "font"
QT_MOC_LITERAL(7, 50, 10), // "selectSize"
QT_MOC_LITERAL(8, 61, 4), // "size"
QT_MOC_LITERAL(9, 66, 11), // "alignCenter"
QT_MOC_LITERAL(10, 78, 9), // "alignLeft"
QT_MOC_LITERAL(11, 88, 10), // "alignRight"
QT_MOC_LITERAL(12, 99, 12), // "alignJustify"
QT_MOC_LITERAL(13, 112, 16) // "updateCharFormat"

    },
    "MainEditor\0Bold\0\0Italic\0Underline\0"
    "selectFont\0font\0selectSize\0size\0"
    "alignCenter\0alignLeft\0alignRight\0"
    "alignJustify\0updateCharFormat"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainEditor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    1,   67,    2, 0x08 /* Private */,
       7,    1,   70,    2, 0x08 /* Private */,
       9,    0,   73,    2, 0x08 /* Private */,
      10,    0,   74,    2, 0x08 /* Private */,
      11,    0,   75,    2, 0x08 /* Private */,
      12,    0,   76,    2, 0x08 /* Private */,
      13,    0,   77,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainEditor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Bold(); break;
        case 1: _t->Italic(); break;
        case 2: _t->Underline(); break;
        case 3: _t->selectFont((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->selectSize((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->alignCenter(); break;
        case 6: _t->alignLeft(); break;
        case 7: _t->alignRight(); break;
        case 8: _t->alignJustify(); break;
        case 9: _t->updateCharFormat(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainEditor::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainEditor.data,
    qt_meta_data_MainEditor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainEditor.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
