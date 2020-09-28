/****************************************************************************
** Meta object code from reading C++ file 'AnimaticWidow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AnimaticWidow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AnimaticWidow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AnimaticWidow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   15,   14,   14, 0x0a,
      58,   14,   14,   14, 0x0a,
      83,   79,   14,   14, 0x0a,
     118,  111,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AnimaticWidow[] = {
    "AnimaticWidow\0\0action\0"
    "Slot_AnimaticToolBarEvent(QAction*)\0"
    "Slot_ButtonClicked()\0dir\0"
    "AddTableWidgetItem(QString)\0iIndex\0"
    "Slot_ComboBoxIndex(int)\0"
};

void AnimaticWidow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AnimaticWidow *_t = static_cast<AnimaticWidow *>(_o);
        switch (_id) {
        case 0: _t->Slot_AnimaticToolBarEvent((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: _t->Slot_ButtonClicked(); break;
        case 2: _t->AddTableWidgetItem((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->Slot_ComboBoxIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AnimaticWidow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AnimaticWidow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_AnimaticWidow,
      qt_meta_data_AnimaticWidow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AnimaticWidow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AnimaticWidow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AnimaticWidow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AnimaticWidow))
        return static_cast<void*>(const_cast< AnimaticWidow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int AnimaticWidow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
