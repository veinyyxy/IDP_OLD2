/****************************************************************************
** Meta object code from reading C++ file 'CipasLegendModel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasLegendModel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasLegendModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCipasLegendModel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,   35,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CCipasLegendModel[] = {
    "CCipasLegendModel\0\0layersChanged()\0"
    "layerId\0removeLayer(QString)\0"
};

void CCipasLegendModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCipasLegendModel *_t = static_cast<CCipasLegendModel *>(_o);
        switch (_id) {
        case 0: _t->layersChanged(); break;
        case 1: _t->removeLayer((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCipasLegendModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCipasLegendModel::staticMetaObject = {
    { &QStandardItemModel::staticMetaObject, qt_meta_stringdata_CCipasLegendModel,
      qt_meta_data_CCipasLegendModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCipasLegendModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCipasLegendModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCipasLegendModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCipasLegendModel))
        return static_cast<void*>(const_cast< CCipasLegendModel*>(this));
    return QStandardItemModel::qt_metacast(_clname);
}

int CCipasLegendModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QStandardItemModel::qt_metacall(_c, _id, _a);
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
void CCipasLegendModel::layersChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
