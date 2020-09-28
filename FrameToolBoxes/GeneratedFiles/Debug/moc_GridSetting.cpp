/****************************************************************************
** Meta object code from reading C++ file 'GridSetting.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "../../GridSetting.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GridSetting.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GridSetting[] = {

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
      13,   12,   12,   12, 0x0a,
      32,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_GridSetting[] = {
    "GridSetting\0\0Slots_OnOKCliked()\0"
    "Slots_OnQuitCliked()\0"
};

void GridSetting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GridSetting *_t = static_cast<GridSetting *>(_o);
        switch (_id) {
        case 0: _t->Slots_OnOKCliked(); break;
        case 1: _t->Slots_OnQuitCliked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData GridSetting::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GridSetting::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_GridSetting,
      qt_meta_data_GridSetting, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GridSetting::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GridSetting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GridSetting::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GridSetting))
        return static_cast<void*>(const_cast< GridSetting*>(this));
    return QDialog::qt_metacast(_clname);
}

int GridSetting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
static const uint qt_meta_data_ProjCenterSetting[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ProjCenterSetting[] = {
    "ProjCenterSetting\0\0Slots_OnOKCliked()\0"
};

void ProjCenterSetting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ProjCenterSetting *_t = static_cast<ProjCenterSetting *>(_o);
        switch (_id) {
        case 0: _t->Slots_OnOKCliked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ProjCenterSetting::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ProjCenterSetting::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ProjCenterSetting,
      qt_meta_data_ProjCenterSetting, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ProjCenterSetting::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ProjCenterSetting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ProjCenterSetting::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ProjCenterSetting))
        return static_cast<void*>(const_cast< ProjCenterSetting*>(this));
    return QDialog::qt_metacast(_clname);
}

int ProjCenterSetting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
