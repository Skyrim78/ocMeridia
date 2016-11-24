/****************************************************************************
** Meta object code from reading C++ file 'product_form.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ocMeridia/product_form.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'product_form.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_product_form_t {
    QByteArrayData data[12];
    char stringdata[130];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_product_form_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_product_form_t qt_meta_stringdata_product_form = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 10),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 16),
QT_MOC_LITERAL(4, 42, 14),
QT_MOC_LITERAL(5, 57, 13),
QT_MOC_LITERAL(6, 71, 15),
QT_MOC_LITERAL(7, 87, 18),
QT_MOC_LITERAL(8, 106, 4),
QT_MOC_LITERAL(9, 111, 11),
QT_MOC_LITERAL(10, 123, 3),
QT_MOC_LITERAL(11, 127, 1)
    },
    "product_form\0loadGroups\0\0loadFileProducts\0"
    "loadOCProducts\0selectProduct\0"
    "unselectProduct\0checkSelectionMode\0"
    "save\0makeMessage\0str\0x\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_product_form[] = {

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
       1,    0,   54,    2, 0x0a,
       3,    0,   55,    2, 0x0a,
       4,    0,   56,    2, 0x0a,
       5,    0,   57,    2, 0x0a,
       6,    0,   58,    2, 0x0a,
       7,    0,   59,    2, 0x0a,
       8,    0,   60,    2, 0x0a,
       9,    2,   61,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   10,   11,

       0        // eod
};

void product_form::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        product_form *_t = static_cast<product_form *>(_o);
        switch (_id) {
        case 0: _t->loadGroups(); break;
        case 1: _t->loadFileProducts(); break;
        case 2: _t->loadOCProducts(); break;
        case 3: _t->selectProduct(); break;
        case 4: _t->unselectProduct(); break;
        case 5: _t->checkSelectionMode(); break;
        case 6: _t->save(); break;
        case 7: _t->makeMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject product_form::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_product_form.data,
      qt_meta_data_product_form,  qt_static_metacall, 0, 0}
};


const QMetaObject *product_form::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *product_form::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_product_form.stringdata))
        return static_cast<void*>(const_cast< product_form*>(this));
    return QDialog::qt_metacast(_clname);
}

int product_form::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
