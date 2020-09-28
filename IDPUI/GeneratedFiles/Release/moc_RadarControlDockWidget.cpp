/****************************************************************************
** Meta object code from reading C++ file 'RadarControlDockWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../RadarControlDockWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RadarControlDockWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RadarControlDockWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      32,   24,   23,   23, 0x0a,
      55,   49,   23,   23, 0x0a,
      84,   76,   23,   23, 0x0a,
     116,   76,   23,   23, 0x0a,
     148,   76,   23,   23, 0x0a,
     177,   76,   23,   23, 0x0a,
     206,   76,   23,   23, 0x0a,
     241,  235,   23,   23, 0x0a,
     264,  235,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_RadarControlDockWidget[] = {
    "RadarControlDockWidget\0\0visible\0"
    "setVisible(bool)\0index\0Slot_ChangLayer(int)\0"
    "checked\0Slot_IsoSurface1_Clicked1(bool)\0"
    "Slot_IsoSurface1_Clicked2(bool)\0"
    "Slot_Section1_Clicked1(bool)\0"
    "Slot_Section2_Clicked2(bool)\0"
    "Slot_Section3_Clicked3(bool)\0value\0"
    "Slot_Slider1Moved(int)\0Slot_Slider2Moved(int)\0"
};

void RadarControlDockWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RadarControlDockWidget *_t = static_cast<RadarControlDockWidget *>(_o);
        switch (_id) {
        case 0: _t->setVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->Slot_ChangLayer((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->Slot_IsoSurface1_Clicked1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->Slot_IsoSurface1_Clicked2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->Slot_Section1_Clicked1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->Slot_Section2_Clicked2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->Slot_Section3_Clicked3((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->Slot_Slider1Moved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->Slot_Slider2Moved((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RadarControlDockWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RadarControlDockWidget::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_RadarControlDockWidget,
      qt_meta_data_RadarControlDockWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RadarControlDockWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RadarControlDockWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RadarControlDockWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RadarControlDockWidget))
        return static_cast<void*>(const_cast< RadarControlDockWidget*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int RadarControlDockWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
