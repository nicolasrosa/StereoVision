/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../reprojectImageTo3D_calibON_bm_GUI/mainwindow.h"
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
    char stringdata[291];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 9),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 16),
QT_MOC_LITERAL(4, 39, 8),
QT_MOC_LITERAL(5, 48, 17),
QT_MOC_LITERAL(6, 66, 4),
QT_MOC_LITERAL(7, 71, 4),
QT_MOC_LITERAL(8, 76, 8),
QT_MOC_LITERAL(9, 85, 6),
QT_MOC_LITERAL(10, 92, 6),
QT_MOC_LITERAL(11, 99, 31),
QT_MOC_LITERAL(12, 131, 27),
QT_MOC_LITERAL(13, 159, 30),
QT_MOC_LITERAL(14, 190, 34),
QT_MOC_LITERAL(15, 225, 34),
QT_MOC_LITERAL(16, 260, 29)
    },
    "MainWindow\0printHelp\0\0openStereoSource\0"
    "inputNum\0cv::VideoCapture*\0capL\0capR\0"
    "cv::Mat*\0imageL\0imageR\0"
    "StereoVisionProcessAndUpdateGUI\0"
    "on_btnPauseOrResume_clicked\0"
    "on_btnShowDisparityMap_clicked\0"
    "on_btnShowStereoParamSetup_clicked\0"
    "on_btnShow3DReconstruction_clicked\0"
    "on_btnShowInputImages_clicked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

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
       3,    5,   55,    2, 0x0a,
      11,    0,   66,    2, 0x0a,
      12,    0,   67,    2, 0x08,
      13,    0,   68,    2, 0x08,
      14,    0,   69,    2, 0x08,
      15,    0,   70,    2, 0x08,
      16,    0,   71,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 5, 0x80000000 | 5, 0x80000000 | 8, 0x80000000 | 8,    4,    6,    7,    9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->printHelp(); break;
        case 1: _t->openStereoSource((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< cv::VideoCapture*(*)>(_a[2])),(*reinterpret_cast< cv::VideoCapture*(*)>(_a[3])),(*reinterpret_cast< cv::Mat*(*)>(_a[4])),(*reinterpret_cast< cv::Mat*(*)>(_a[5]))); break;
        case 2: _t->StereoVisionProcessAndUpdateGUI(); break;
        case 3: _t->on_btnPauseOrResume_clicked(); break;
        case 4: _t->on_btnShowDisparityMap_clicked(); break;
        case 5: _t->on_btnShowStereoParamSetup_clicked(); break;
        case 6: _t->on_btnShow3DReconstruction_clicked(); break;
        case 7: _t->on_btnShowInputImages_clicked(); break;
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
