/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../inc/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata[431];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 29),
QT_MOC_LITERAL(2, 41, 0),
QT_MOC_LITERAL(3, 42, 27),
QT_MOC_LITERAL(4, 70, 30),
QT_MOC_LITERAL(5, 101, 34),
QT_MOC_LITERAL(6, 136, 34),
QT_MOC_LITERAL(7, 171, 29),
QT_MOC_LITERAL(8, 201, 36),
QT_MOC_LITERAL(9, 238, 27),
QT_MOC_LITERAL(10, 266, 30),
QT_MOC_LITERAL(11, 297, 28),
QT_MOC_LITERAL(12, 326, 7),
QT_MOC_LITERAL(13, 334, 27),
QT_MOC_LITERAL(14, 362, 33),
QT_MOC_LITERAL(15, 396, 27),
QT_MOC_LITERAL(16, 424, 5)
    },
    "MainWindow\0stereoVisionProcess_UpdateGUI\0"
    "\0on_btnPauseOrResume_clicked\0"
    "on_btnShowDisparityMap_clicked\0"
    "on_btnShowStereoParamSetup_clicked\0"
    "on_btnShow3DReconstruction_clicked\0"
    "on_btnShowInputImages_clicked\0"
    "on_btnShowTrackingObjectView_clicked\0"
    "on_btnShowDiffImage_clicked\0"
    "on_btnShowWarningLines_clicked\0"
    "on_toggleBtnShowHist_clicked\0checked\0"
    "on_toggleBtnShowXYZ_toggled\0"
    "on_toggleBtnShowDispDepth_toggled\0"
    "on_methodSelector_activated\0index\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x0a,
       3,    0,   80,    2, 0x08,
       4,    0,   81,    2, 0x08,
       5,    0,   82,    2, 0x08,
       6,    0,   83,    2, 0x08,
       7,    0,   84,    2, 0x08,
       8,    0,   85,    2, 0x08,
       9,    0,   86,    2, 0x08,
      10,    0,   87,    2, 0x08,
      11,    1,   88,    2, 0x08,
      13,    1,   91,    2, 0x08,
      14,    1,   94,    2, 0x08,
      15,    1,   97,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Int,   16,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->StereoVisionProcess_UpdateGUI(); break;
        case 1: _t->on_btnPauseOrResume_clicked(); break;
        case 2: _t->on_btnShowDisparityMap_clicked(); break;
        case 3: _t->on_btnShowStereoParamSetup_clicked(); break;
        case 4: _t->on_btnShow3DReconstruction_clicked(); break;
        case 5: _t->on_btnShowInputImages_clicked(); break;
        case 6: _t->on_btnShowTrackingObjectView_clicked(); break;
        case 7: _t->on_btnShowDiffImage_clicked(); break;
        case 8: _t->on_btnShowWarningLines_clicked(); break;
        case 9: _t->on_toggleBtnShowHist_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->on_toggleBtnShowXYZ_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->on_toggleBtnShowDispDepth_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->on_methodSelector_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
