/****************************************************************************
** Meta object code from reading C++ file 'bcevaluesetplot.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hpp/bcevaluesetplot.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bcevaluesetplot.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BCEValueSetPlot_t {
    QByteArrayData data[9];
    char stringdata[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_BCEValueSetPlot_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_BCEValueSetPlot_t qt_meta_stringdata_BCEValueSetPlot = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 17),
QT_MOC_LITERAL(2, 34, 0),
QT_MOC_LITERAL(3, 35, 1),
QT_MOC_LITERAL(4, 37, 1),
QT_MOC_LITERAL(5, 39, 16),
QT_MOC_LITERAL(6, 56, 12),
QT_MOC_LITERAL(7, 69, 5),
QT_MOC_LITERAL(8, 75, 19)
    },
    "BCEValueSetPlot\0newEqmCoordinates\0\0x\0"
    "y\0showPointToolTip\0QMouseEvent*\0event\0"
    "getClickCoordinates\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BCEValueSetPlot[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       5,    1,   34,    2, 0x0a,
       8,    1,   37,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

void BCEValueSetPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BCEValueSetPlot *_t = static_cast<BCEValueSetPlot *>(_o);
        switch (_id) {
        case 0: _t->newEqmCoordinates((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->showPointToolTip((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: _t->getClickCoordinates((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BCEValueSetPlot::*_t)(double , double );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BCEValueSetPlot::newEqmCoordinates)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject BCEValueSetPlot::staticMetaObject = {
    { &QCustomPlot::staticMetaObject, qt_meta_stringdata_BCEValueSetPlot.data,
      qt_meta_data_BCEValueSetPlot,  qt_static_metacall, 0, 0}
};


const QMetaObject *BCEValueSetPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BCEValueSetPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BCEValueSetPlot.stringdata))
        return static_cast<void*>(const_cast< BCEValueSetPlot*>(this));
    return QCustomPlot::qt_metacast(_clname);
}

int BCEValueSetPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCustomPlot::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void BCEValueSetPlot::newEqmCoordinates(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
