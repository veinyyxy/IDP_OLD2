/****************************************************************************
** Meta object code from reading C++ file 'DataDockWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DataDockWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DataDockWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DataDockWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   16,   15,   15, 0x0a,
      41,   15,   15,   15, 0x0a,
      72,   66,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DataDockWidget[] = {
    "DataDockWidget\0\0visible\0setVisible(bool)\0"
    "Slot_PushButtonClicked()\0index\0"
    "Slot_DoubleClicked(QModelIndex)\0"
};

void DataDockWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DataDockWidget *_t = static_cast<DataDockWidget *>(_o);
        switch (_id) {
        case 0: _t->setVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->Slot_PushButtonClicked(); break;
        case 2: _t->Slot_DoubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DataDockWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DataDockWidget::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_DataDockWidget,
      qt_meta_data_DataDockWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DataDockWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DataDockWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DataDockWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DataDockWidget))
        return static_cast<void*>(const_cast< DataDockWidget*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int DataDockWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE