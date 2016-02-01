/****************************************************************************
** Meta object code from reading C++ file 'opencview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "opencview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'opencview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_OpenCview_t {
    QByteArrayData data[12];
    char stringdata[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_OpenCview_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_OpenCview_t qt_meta_stringdata_OpenCview = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 10),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 12),
QT_MOC_LITERAL(4, 35, 1),
QT_MOC_LITERAL(5, 37, 10),
QT_MOC_LITERAL(6, 48, 12),
QT_MOC_LITERAL(7, 61, 15),
QT_MOC_LITERAL(8, 77, 12),
QT_MOC_LITERAL(9, 90, 17),
QT_MOC_LITERAL(10, 108, 14),
QT_MOC_LITERAL(11, 123, 20)
    },
    "OpenCview\0paintEvent\0\0QPaintEvent*\0e\0"
    "timerEvent\0QTimerEvent*\0mousePressEvent\0"
    "QMouseEvent*\0mouseReleaseEvent\0"
    "mouseMoveEvent\0enable_ROI_selection\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OpenCview[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a,
       5,    1,   47,    2, 0x0a,
       7,    1,   50,    2, 0x0a,
       9,    1,   53,    2, 0x0a,
      10,    1,   56,    2, 0x0a,
      11,    0,   59,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    4,
    QMetaType::Void, 0x80000000 | 8,    4,
    QMetaType::Void, 0x80000000 | 8,    4,
    QMetaType::Void, 0x80000000 | 8,    4,
    QMetaType::Void,

       0        // eod
};

void OpenCview::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        OpenCview *_t = static_cast<OpenCview *>(_o);
        switch (_id) {
        case 0: _t->paintEvent((*reinterpret_cast< QPaintEvent*(*)>(_a[1]))); break;
        case 1: _t->timerEvent((*reinterpret_cast< QTimerEvent*(*)>(_a[1]))); break;
        case 2: _t->mousePressEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: _t->mouseReleaseEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 4: _t->mouseMoveEvent((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 5: _t->enable_ROI_selection(); break;
        default: ;
        }
    }
}

const QMetaObject OpenCview::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_OpenCview.data,
      qt_meta_data_OpenCview,  qt_static_metacall, 0, 0}
};


const QMetaObject *OpenCview::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OpenCview::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_OpenCview.stringdata))
        return static_cast<void*>(const_cast< OpenCview*>(this));
    return QWidget::qt_metacast(_clname);
}

int OpenCview::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
