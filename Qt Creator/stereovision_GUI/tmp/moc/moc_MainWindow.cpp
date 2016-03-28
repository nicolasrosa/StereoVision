/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../inc/MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[17];
    char stringdata0[430];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 29), // "stereoVisionProcess_UpdateGUI"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 27), // "on_btnPauseOrResume_clicked"
QT_MOC_LITERAL(4, 70, 30), // "on_btnShowDisparityMap_clicked"
QT_MOC_LITERAL(5, 101, 34), // "on_btnShowStereoParamSetup_cl..."
QT_MOC_LITERAL(6, 136, 34), // "on_btnShow3DReconstruction_cl..."
QT_MOC_LITERAL(7, 171, 29), // "on_btnShowInputImages_clicked"
QT_MOC_LITERAL(8, 201, 36), // "on_btnShowTrackingObjectView_..."
QT_MOC_LITERAL(9, 238, 27), // "on_btnShowDiffImage_clicked"
QT_MOC_LITERAL(10, 266, 30), // "on_btnShowWarningLines_clicked"
QT_MOC_LITERAL(11, 297, 28), // "on_toggleBtnShowHist_clicked"
QT_MOC_LITERAL(12, 326, 7), // "checked"
QT_MOC_LITERAL(13, 334, 27), // "on_toggleBtnShowXYZ_toggled"
QT_MOC_LITERAL(14, 362, 33), // "on_toggleBtnShowDispDepth_tog..."
QT_MOC_LITERAL(15, 396, 27), // "on_methodSelector_activated"
QT_MOC_LITERAL(16, 424, 5) // "index"

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
    "on_methodSelector_activated\0index"
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
       1,    0,   79,    2, 0x0a /* Public */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    0,   86,    2, 0x08 /* Private */,
      10,    0,   87,    2, 0x08 /* Private */,
      11,    1,   88,    2, 0x08 /* Private */,
      13,    1,   91,    2, 0x08 /* Private */,
      14,    1,   94,    2, 0x08 /* Private */,
      15,    1,   97,    2, 0x08 /* Private */,

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
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->stereoVisionProcess_UpdateGUI(); break;
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
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
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
