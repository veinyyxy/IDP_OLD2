/****************************************************************************
** Meta object code from reading C++ file 'CommonToolBar.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CommonToolBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CommonToolBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CommonToolBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   15,   14,   14, 0x0a,
      47,   40,   14,   14, 0x0a,
      79,   14,   14,   14, 0x0a,
      89,   14,   14,   14, 0x0a,
     101,   14,   14,   14, 0x0a,
     134,  114,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CommonToolBar[] = {
    "CommonToolBar\0\0visible\0setVisible(bool)\0"
    "action\0Slots_ActionTriggered(QAction*)\0"
    "MapFull()\0MapZoonIn()\0MapZoonOut()\0"
    "beginPoint,endPoint\0eMapMove(QPoint,QPoint)\0"
};

void CommonToolBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CommonToolBar *_t = static_cast<CommonToolBar *>(_o);
        switch (_id) {
        case 0: _t->setVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->Slots_ActionTriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 2: _t->MapFull(); break;
        case 3: _t->MapZoonIn(); break;
        case 4: _t->MapZoonOut(); break;
        case 5: _t->eMapMove((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< QPoint(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CommonToolBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CommonToolBar::staticMetaObject = {
    { &QToolBar::staticMetaObject, qt_meta_stringdata_CommonToolBar,
      qt_meta_data_CommonToolBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CommonToolBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CommonToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CommonToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CommonToolBar))
        return static_cast<void*>(const_cast< CommonToolBar*>(this));
    return QToolBar::qt_metacast(_clname);
}

int CommonToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
