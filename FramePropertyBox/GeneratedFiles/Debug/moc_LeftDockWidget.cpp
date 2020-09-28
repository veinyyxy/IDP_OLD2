/****************************************************************************
** Meta object code from reading C++ file 'LeftDockWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../LeftDockWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LeftDockWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LeftDockWidget[] = {

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
      24,   16,   15,   15, 0x0a,
      47,   41,   15,   15, 0x0a,
      81,   75,   15,   15, 0x0a,
     110,   41,   15,   15, 0x0a,
     146,   15,   15,   15, 0x0a,
     173,   15,   15,   15, 0x0a,
     187,   15,   15,   15, 0x0a,
     203,   15,   15,   15, 0x0a,
     228,  221,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LeftDockWidget[] = {
    "LeftDockWidget\0\0visible\0setVisible(bool)\0"
    "index\0Slot_ItemClick(QModelIndex)\0"
    "point\0Slot_ShowContextMenu(QPoint)\0"
    "Slot_itemDoubleClicked(QModelIndex)\0"
    "Slot_TreeRightMenuAction()\0Slot_MoveUp()\0"
    "Slot_MoveDown()\0Slot_AddBaseMap()\0"
    "action\0Slot_ActionTriggered_dock(QAction*)\0"
};

void LeftDockWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LeftDockWidget *_t = static_cast<LeftDockWidget *>(_o);
        switch (_id) {
        case 0: _t->setVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->Slot_ItemClick((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->Slot_ShowContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 3: _t->Slot_itemDoubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 4: _t->Slot_TreeRightMenuAction(); break;
        case 5: _t->Slot_MoveUp(); break;
        case 6: _t->Slot_MoveDown(); break;
        case 7: _t->Slot_AddBaseMap(); break;
        case 8: _t->Slot_ActionTriggered_dock((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LeftDockWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LeftDockWidget::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_LeftDockWidget,
      qt_meta_data_LeftDockWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LeftDockWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LeftDockWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LeftDockWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LeftDockWidget))
        return static_cast<void*>(const_cast< LeftDockWidget*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int LeftDockWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
