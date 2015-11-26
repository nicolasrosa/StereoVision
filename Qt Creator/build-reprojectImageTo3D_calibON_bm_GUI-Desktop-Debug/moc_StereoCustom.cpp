/****************************************************************************
** Meta object code from reading C++ file 'StereoCustom.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../reprojectImageTo3D_calibON_bm_GUI/StereoCustom.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StereoCustom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_StereoProcessor_t {
    QByteArrayData data[13];
    char stringdata[183];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_StereoProcessor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_StereoProcessor_t qt_meta_stringdata_StereoProcessor = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 9),
QT_MOC_LITERAL(2, 26, 0),
QT_MOC_LITERAL(3, 27, 13),
QT_MOC_LITERAL(4, 41, 12),
QT_MOC_LITERAL(5, 54, 13),
QT_MOC_LITERAL(6, 68, 12),
QT_MOC_LITERAL(7, 81, 16),
QT_MOC_LITERAL(8, 98, 16),
QT_MOC_LITERAL(9, 115, 15),
QT_MOC_LITERAL(10, 131, 17),
QT_MOC_LITERAL(11, 149, 18),
QT_MOC_LITERAL(12, 168, 13)
    },
    "StereoProcessor\0setValues\0\0preFilterSize\0"
    "preFilterCap\0sadWindowSize\0minDisparity\0"
    "numOfDisparities\0textureThreshold\0"
    "uniquenessRatio\0speckleWindowSize\0"
    "speckleWindowRange\0disp12MaxDiff\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_StereoProcessor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,   10,   19,    2, 0x0a,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,

       0        // eod
};

void StereoProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        StereoProcessor *_t = static_cast<StereoProcessor *>(_o);
        switch (_id) {
        case 0: _t->setValues((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8])),(*reinterpret_cast< int(*)>(_a[9])),(*reinterpret_cast< int(*)>(_a[10]))); break;
        default: ;
        }
    }
}

const QMetaObject StereoProcessor::staticMetaObject = {
    { &Ui_SetStereoParams::staticMetaObject, qt_meta_stringdata_StereoProcessor.data,
      qt_meta_data_StereoProcessor,  qt_static_metacall, 0, 0}
};


const QMetaObject *StereoProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *StereoProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StereoProcessor.stringdata))
        return static_cast<void*>(const_cast< StereoProcessor*>(this));
    if (!strcmp(_clname, "StereoConfig"))
        return static_cast< StereoConfig*>(const_cast< StereoProcessor*>(this));
    if (!strcmp(_clname, "QObject"))
        return static_cast< QObject*>(const_cast< StereoProcessor*>(this));
    return Ui_SetStereoParams::qt_metacast(_clname);
}

int StereoProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Ui_SetStereoParams::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
