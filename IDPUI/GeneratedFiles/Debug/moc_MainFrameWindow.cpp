/****************************************************************************
** Meta object code from reading C++ file 'MainFrameWindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MainFrameWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainFrameWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainFrameWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   17,   16,   16, 0x0a,
      62,   55,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainFrameWindow[] = {
    "MainFrameWindow\0\0action\0"
    "slot_ActionTriggered(QAction*)\0window\0"
    "slot_subWindowActivated(QMdiSubWindow*)\0"
};

void MainFrameWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainFrameWindow *_t = static_cast<MainFrameWindow *>(_o);
        switch (_id) {
        case 0: _t->slot_ActionTriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: _t->slot_subWindowActivated((*reinterpret_cast< QMdiSubWindow*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainFrameWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainFrameWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainFrameWindow,
      qt_meta_data_MainFrameWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainFrameWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainFrameWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainFrameWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainFrameWindow))
        return static_cast<void*>(const_cast< MainFrameWindow*>(this));
    if (!strcmp(_clname, "UpdateMasterInterface"))
        return static_cast< UpdateMasterInterface*>(const_cast< MainFrameWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainFrameWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
