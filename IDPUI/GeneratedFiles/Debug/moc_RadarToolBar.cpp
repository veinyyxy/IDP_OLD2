/****************************************************************************
** Meta object code from reading C++ file 'RadarToolBar.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../RadarToolBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RadarToolBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RadarToolBar[] = {

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
      21,   14,   13,   13, 0x0a,
      60,   53,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RadarToolBar[] = {
    "RadarToolBar\0\0action\0"
    "Slots_ActionTriggered(QAction*)\0widget\0"
    "Slot_ActivateWindow(QWidget*)\0"
};

void RadarToolBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RadarToolBar *_t = static_cast<RadarToolBar *>(_o);
        switch (_id) {
        case 0: _t->Slots_ActionTriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: _t->Slot_ActivateWindow((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RadarToolBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RadarToolBar::staticMetaObject = {
    { &QToolBar::staticMetaObject, qt_meta_stringdata_RadarToolBar,
      qt_meta_data_RadarToolBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RadarToolBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RadarToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RadarToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RadarToolBar))
        return static_cast<void*>(const_cast< RadarToolBar*>(this));
    return QToolBar::qt_metacast(_clname);
}

int RadarToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolBar::qt_metacall(_c, _id, _a);
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
