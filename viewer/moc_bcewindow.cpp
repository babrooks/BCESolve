/****************************************************************************
** Meta object code from reading C++ file 'bcewindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hpp/bcewindow.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bcewindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BCEWindow_t {
    QByteArrayData data[17];
    char stringdata[174];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_BCEWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_BCEWindow_t qt_meta_stringdata_BCEWindow = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 17),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 6),
QT_MOC_LITERAL(4, 36, 13),
QT_MOC_LITERAL(5, 50, 4),
QT_MOC_LITERAL(6, 55, 6),
QT_MOC_LITERAL(7, 62, 18),
QT_MOC_LITERAL(8, 81, 7),
QT_MOC_LITERAL(9, 89, 9),
QT_MOC_LITERAL(10, 99, 1),
QT_MOC_LITERAL(11, 101, 1),
QT_MOC_LITERAL(12, 103, 12),
QT_MOC_LITERAL(13, 116, 14),
QT_MOC_LITERAL(14, 131, 11),
QT_MOC_LITERAL(15, 143, 16),
QT_MOC_LITERAL(16, 160, 12)
    },
    "BCEWindow\0changeSliderValue\0\0newVal\0"
    "BCESliderType\0type\0player\0changeMarginalBool\0"
    "newBool\0setNewEqm\0x\0y\0loadSolution\0"
    "setLinearScale\0setLogScale\0setColorfulTheme\0"
    "setBlueTheme\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BCEWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   54,    2, 0x0a,
       7,    3,   61,    2, 0x0a,
       9,    2,   68,    2, 0x0a,
      12,    0,   73,    2, 0x08,
      13,    0,   74,    2, 0x08,
      14,    0,   75,    2, 0x08,
      15,    0,   76,    2, 0x08,
      16,    0,   77,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4, QMetaType::Int,    3,    5,    6,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 4, QMetaType::Int,    8,    5,    6,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BCEWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BCEWindow *_t = static_cast<BCEWindow *>(_o);
        switch (_id) {
        case 0: _t->changeSliderValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< BCESliderType(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->changeMarginalBool((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< BCESliderType(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->setNewEqm((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->loadSolution(); break;
        case 4: _t->setLinearScale(); break;
        case 5: _t->setLogScale(); break;
        case 6: _t->setColorfulTheme(); break;
        case 7: _t->setBlueTheme(); break;
        default: ;
        }
    }
}

const QMetaObject BCEWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_BCEWindow.data,
      qt_meta_data_BCEWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *BCEWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BCEWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BCEWindow.stringdata))
        return static_cast<void*>(const_cast< BCEWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int BCEWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
