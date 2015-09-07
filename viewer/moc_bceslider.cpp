/****************************************************************************
** Meta object code from reading C++ file 'bceslider.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hpp/bceslider.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bceslider.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BCESlider_t {
    QByteArrayData data[12];
    char stringdata[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_BCESlider_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_BCESlider_t qt_meta_stringdata_BCESlider = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 12),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 8),
QT_MOC_LITERAL(4, 33, 13),
QT_MOC_LITERAL(5, 47, 4),
QT_MOC_LITERAL(6, 52, 6),
QT_MOC_LITERAL(7, 59, 11),
QT_MOC_LITERAL(8, 71, 20),
QT_MOC_LITERAL(9, 92, 13),
QT_MOC_LITERAL(10, 106, 11),
QT_MOC_LITERAL(11, 118, 13)
    },
    "BCESlider\0valueChanged\0\0newValue\0"
    "BCESliderType\0type\0player\0changeValue\0"
    "changeSliderPosition\0newValEmitted\0"
    "typeEmitted\0playerEmitted\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BCESlider[] = {

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
       1,    3,   29,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       7,    1,   36,    2, 0x0a,
       8,    3,   39,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4, QMetaType::Int,    3,    5,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4, QMetaType::Int,    9,   10,   11,

       0        // eod
};

void BCESlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BCESlider *_t = static_cast<BCESlider *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< BCESliderType(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->changeValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->changeSliderPosition((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< BCESliderType(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BCESlider::*_t)(int , BCESliderType , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BCESlider::valueChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject BCESlider::staticMetaObject = {
    { &QSlider::staticMetaObject, qt_meta_stringdata_BCESlider.data,
      qt_meta_data_BCESlider,  qt_static_metacall, 0, 0}
};


const QMetaObject *BCESlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BCESlider::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BCESlider.stringdata))
        return static_cast<void*>(const_cast< BCESlider*>(this));
    return QSlider::qt_metacast(_clname);
}

int BCESlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSlider::qt_metacall(_c, _id, _a);
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
void BCESlider::valueChanged(int _t1, BCESliderType _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
