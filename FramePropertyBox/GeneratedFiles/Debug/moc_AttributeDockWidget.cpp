/****************************************************************************
** Meta object code from reading C++ file 'AttributeDockWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../AttributeDockWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AttributeDockWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AttributeDockWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   21,   20,   20, 0x0a,
      59,   46,   20,   20, 0x0a,
      94,   46,   20,   20, 0x0a,
     137,   46,   20,   20, 0x0a,
     174,   46,   20,   20, 0x0a,
     212,   46,   20,   20, 0x0a,
     249,   46,   20,   20, 0x0a,
     287,   46,   20,   20, 0x0a,
     326,   46,   20,   20, 0x0a,
     364,   46,   20,   20, 0x0a,
     400,   46,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AttributeDockWidget[] = {
    "AttributeDockWidget\0\0visible\0"
    "setVisible(bool)\0property,val\0"
    "Slot_ValueChanged(QtProperty*,int)\0"
    "Slot_ValueChanged(QtProperty*,QStringList)\0"
    "Slot_ValueChanged(QtProperty*,QSize)\0"
    "Slot_ValueChanged(QtProperty*,QPoint)\0"
    "Slot_ValueChanged(QtProperty*,QFont)\0"
    "Slot_ValueChanged(QtProperty*,QColor)\0"
    "Slot_ValueChanged(QtProperty*,QString)\0"
    "Slot_ValueChanged(QtProperty*,double)\0"
    "Slot_ValueChanged(QtProperty*,bool)\0"
    "UpdateProperty(QtProperty*,QVariant&)\0"
};

void AttributeDockWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AttributeDockWidget *_t = static_cast<AttributeDockWidget *>(_o);
        switch (_id) {
        case 0: _t->setVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->Slot_ValueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->Slot_ValueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 3: _t->Slot_ValueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QSize(*)>(_a[2]))); break;
        case 4: _t->Slot_ValueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2]))); break;
        case 5: _t->Slot_ValueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QFont(*)>(_a[2]))); break;
        case 6: _t->Slot_ValueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QColor(*)>(_a[2]))); break;
        case 7: _t->Slot_ValueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: _t->Slot_ValueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 9: _t->Slot_ValueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 10: _t->UpdateProperty((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< QVariant(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AttributeDockWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AttributeDockWidget::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_AttributeDockWidget,
      qt_meta_data_AttributeDockWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AttributeDockWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AttributeDockWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AttributeDockWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AttributeDockWidget))
        return static_cast<void*>(const_cast< AttributeDockWidget*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int AttributeDockWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
