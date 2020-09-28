/****************************************************************************
** Meta object code from reading C++ file 'CentralMdiArea.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CentralMdiArea.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CentralMdiArea.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CentralMdiArea[] = {

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
      23,   16,   15,   15, 0x0a,
      77,   63,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CentralMdiArea[] = {
    "CentralMdiArea\0\0window\0"
    "Slot_SubWindowActivated(QMdiSubWindow*)\0"
    "currentWidget\0Slot_ActivateWindow(QWidget*)\0"
};

void CentralMdiArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CentralMdiArea *_t = static_cast<CentralMdiArea *>(_o);
        switch (_id) {
        case 0: _t->Slot_SubWindowActivated((*reinterpret_cast< QMdiSubWindow*(*)>(_a[1]))); break;
        case 1: _t->Slot_ActivateWindow((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CentralMdiArea::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CentralMdiArea::staticMetaObject = {
    { &QMdiArea::staticMetaObject, qt_meta_stringdata_CentralMdiArea,
      qt_meta_data_CentralMdiArea, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CentralMdiArea::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CentralMdiArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CentralMdiArea::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CentralMdiArea))
        return static_cast<void*>(const_cast< CentralMdiArea*>(this));
    return QMdiArea::qt_metacast(_clname);
}

int CentralMdiArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMdiArea::qt_metacall(_c, _id, _a);
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
