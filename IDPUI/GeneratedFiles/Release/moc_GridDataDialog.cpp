/****************************************************************************
** Meta object code from reading C++ file 'GridDataDialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GridDataDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GridDataDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GridDataDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      49,   46,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      89,   82,   15,   15, 0x0a,
     116,   15,   15,   15, 0x0a,
     147,   15,   15,   15, 0x0a,
     176,   15,   15,   15, 0x0a,
     198,  192,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GridDataDialog[] = {
    "GridDataDialog\0\0SendStationItems(QStringList)\0"
    ",,\0SendZoomStation(int,float,float)\0"
    "action\0DataToolBarEvent(QAction*)\0"
    "modifyItemData(QStandardItem*)\0"
    "modifyCipasr(QStandardItem*)\0"
    "FocusStations()\0index\0ZoomToStation(QModelIndex)\0"
};

void GridDataDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GridDataDialog *_t = static_cast<GridDataDialog *>(_o);
        switch (_id) {
        case 0: _t->SendStationItems((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 1: _t->SendZoomStation((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 2: _t->DataToolBarEvent((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 3: _t->modifyItemData((*reinterpret_cast< QStandardItem*(*)>(_a[1]))); break;
        case 4: _t->modifyCipasr((*reinterpret_cast< QStandardItem*(*)>(_a[1]))); break;
        case 5: _t->FocusStations(); break;
        case 6: _t->ZoomToStation((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GridDataDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GridDataDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_GridDataDialog,
      qt_meta_data_GridDataDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GridDataDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GridDataDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GridDataDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GridDataDialog))
        return static_cast<void*>(const_cast< GridDataDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int GridDataDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void GridDataDialog::SendStationItems(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GridDataDialog::SendZoomStation(int _t1, float _t2, float _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
