/****************************************************************************
** Meta object code from reading C++ file 'DefaultToolBar.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DefaultToolBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DefaultToolBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DefaultToolBar[] = {

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
      64,   55,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DefaultToolBar[] = {
    "DefaultToolBar\0\0action\0"
    "Slots_ActionTriggered(QAction*)\0"
    "fileName\0LoadData(QString)\0"
};

void DefaultToolBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DefaultToolBar *_t = static_cast<DefaultToolBar *>(_o);
        switch (_id) {
        case 0: _t->Slots_ActionTriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: _t->LoadData((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData DefaultToolBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject DefaultToolBar::staticMetaObject = {
    { &QToolBar::staticMetaObject, qt_meta_stringdata_DefaultToolBar,
      qt_meta_data_DefaultToolBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DefaultToolBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DefaultToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DefaultToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DefaultToolBar))
        return static_cast<void*>(const_cast< DefaultToolBar*>(this));
    return QToolBar::qt_metacast(_clname);
}

int DefaultToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
