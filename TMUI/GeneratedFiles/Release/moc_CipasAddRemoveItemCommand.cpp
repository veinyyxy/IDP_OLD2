/****************************************************************************
** Meta object code from reading C++ file 'CipasAddRemoveItemCommand.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasAddRemoveItemCommand.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasAddRemoveItemCommand.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCipasAddRemoveItemCommand[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   28,   27,   27, 0x05,
      59,   28,   27,   27, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CCipasAddRemoveItemCommand[] = {
    "CCipasAddRemoveItemCommand\0\0item\0"
    "itemAdded(CComposerItem*)\0"
    "itemRemoved(CComposerItem*)\0"
};

void CCipasAddRemoveItemCommand::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCipasAddRemoveItemCommand *_t = static_cast<CCipasAddRemoveItemCommand *>(_o);
        switch (_id) {
        case 0: _t->itemAdded((*reinterpret_cast< CComposerItem*(*)>(_a[1]))); break;
        case 1: _t->itemRemoved((*reinterpret_cast< CComposerItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCipasAddRemoveItemCommand::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCipasAddRemoveItemCommand::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CCipasAddRemoveItemCommand,
      qt_meta_data_CCipasAddRemoveItemCommand, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCipasAddRemoveItemCommand::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCipasAddRemoveItemCommand::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCipasAddRemoveItemCommand::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCipasAddRemoveItemCommand))
        return static_cast<void*>(const_cast< CCipasAddRemoveItemCommand*>(this));
    if (!strcmp(_clname, "QUndoCommand"))
        return static_cast< QUndoCommand*>(const_cast< CCipasAddRemoveItemCommand*>(this));
    return QObject::qt_metacast(_clname);
}

int CCipasAddRemoveItemCommand::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CCipasAddRemoveItemCommand::itemAdded(CComposerItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CCipasAddRemoveItemCommand::itemRemoved(CComposerItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
