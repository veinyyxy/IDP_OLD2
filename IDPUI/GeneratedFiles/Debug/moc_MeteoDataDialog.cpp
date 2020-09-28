/****************************************************************************
** Meta object code from reading C++ file 'MeteoDataDialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MeteoDataDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MeteoDataDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MeteoDataDialog[] = {

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
      24,   17,   16,   16, 0x0a,
      61,   56,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MeteoDataDialog[] = {
    "MeteoDataDialog\0\0action\0"
    "Slots_ActionTriggered(QAction*)\0text\0"
    "On_CmbVar_CurrentIndexChanged(QString)\0"
};

void MeteoDataDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MeteoDataDialog *_t = static_cast<MeteoDataDialog *>(_o);
        switch (_id) {
        case 0: _t->Slots_ActionTriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: _t->On_CmbVar_CurrentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MeteoDataDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MeteoDataDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MeteoDataDialog,
      qt_meta_data_MeteoDataDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MeteoDataDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MeteoDataDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MeteoDataDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MeteoDataDialog))
        return static_cast<void*>(const_cast< MeteoDataDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int MeteoDataDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
