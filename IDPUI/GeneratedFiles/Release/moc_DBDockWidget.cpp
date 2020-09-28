/****************************************************************************
** Meta object code from reading C++ file 'DBDockWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DBDockWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DBDockWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DBDockWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   14,   13,   13, 0x0a,
      56,   14,   13,   13, 0x0a,
      91,   14,   13,   13, 0x0a,
     129,   14,   13,   13, 0x0a,
     177,  168,   13,   13, 0x0a,
     211,   13,   13,   13, 0x0a,
     236,  231,   13,   13, 0x0a,
     280,   14,   13,   13, 0x0a,
     315,   14,   13,   13, 0x0a,
     353,   14,   13,   13, 0x0a,
     392,  168,   13,   13, 0x0a,
     426,   13,   13,   13, 0x0a,
     446,  231,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DBDockWidget[] = {
    "DBDockWidget\0\0index\0"
    "Slot_AreaCurrentIndexChanged_S(int)\0"
    "Slot_CmbCurrentIndexChanged_S(int)\0"
    "Slot_CmbDayCurrentIndexChanged_S(int)\0"
    "Slot_CmbHourCurrentIndexChanged_S(int)\0"
    "datetime\0Slot_DateTimeChanged_S(QDateTime)\0"
    "Slot_Chbclicked_S()\0item\0"
    "Slot_ItemDoubleClicked_S(QTableWidgetItem*)\0"
    "Slot_CmbCurrentIndexChanged_M(int)\0"
    "Slot_CmbDayCurrentIndexChanged_M(int)\0"
    "Slot_CmbHourCurrentIndexChanged_M(int)\0"
    "Slot_DateTimeChanged_M(QDateTime)\0"
    "Slot_Chbclicked_M()\0"
    "Slot_ItemDoubleClicked_M(QTableWidgetItem*)\0"
};

void DBDockWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DBDockWidget *_t = static_cast<DBDockWidget *>(_o);
        switch (_id) {
        case 0: _t->Slot_AreaCurrentIndexChanged_S((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->Slot_CmbCurrentIndexChanged_S((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->Slot_CmbDayCurrentIndexChanged_S((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->Slot_CmbHourCurrentIndexChanged_S((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->Slot_DateTimeChanged_S((*reinterpret_cast< const QDateTime(*)>(_a[1]))); break;
        case 5: _t->Slot_Chbclicked_S(); break;
        case 6: _t->Slot_ItemDoubleClicked_S((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 7: _t->Slot_CmbCurrentIndexChanged_M((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->Slot_CmbDayCurrentIndexChanged_M((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->Slot_CmbHourCurrentIndexChanged_M((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->Slot_DateTimeChanged_M((*reinterpret_cast< const QDateTime(*)>(_a[1]))); break;
        case 11: _t->Slot_Chbclicked_M(); break;
        case 12: _t->Slot_ItemDoubleClicked_M((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DBDockWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DBDockWidget::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_DBDockWidget,
      qt_meta_data_DBDockWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DBDockWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DBDockWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DBDockWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DBDockWidget))
        return static_cast<void*>(const_cast< DBDockWidget*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int DBDockWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
