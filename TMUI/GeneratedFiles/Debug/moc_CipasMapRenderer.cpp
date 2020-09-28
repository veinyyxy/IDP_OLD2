/****************************************************************************
** Meta object code from reading C++ file 'CipasMapRenderer.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasMapRenderer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasMapRenderer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCipasMapRenderer[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      33,   19,   18,   18, 0x05,
      63,   58,   18,   18, 0x05,
      92,   18,   18,   18, 0x05,
     116,   18,   18,   18, 0x05,
     128,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
     146,   19,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CCipasMapRenderer[] = {
    "CCipasMapRenderer\0\0current,total\0"
    "drawingProgress(int,int)\0flag\0"
    "hasCrsTransformEnabled(bool)\0"
    "destinationSrsChanged()\0updateMap()\0"
    "mapUnitsChanged()\0onDrawingProgress(int,int)\0"
};

void CCipasMapRenderer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCipasMapRenderer *_t = static_cast<CCipasMapRenderer *>(_o);
        switch (_id) {
        case 0: _t->drawingProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->hasCrsTransformEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->destinationSrsChanged(); break;
        case 3: _t->updateMap(); break;
        case 4: _t->mapUnitsChanged(); break;
        case 5: _t->onDrawingProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCipasMapRenderer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCipasMapRenderer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CCipasMapRenderer,
      qt_meta_data_CCipasMapRenderer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCipasMapRenderer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCipasMapRenderer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCipasMapRenderer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCipasMapRenderer))
        return static_cast<void*>(const_cast< CCipasMapRenderer*>(this));
    return QObject::qt_metacast(_clname);
}

int CCipasMapRenderer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CCipasMapRenderer::drawingProgress(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CCipasMapRenderer::hasCrsTransformEnabled(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CCipasMapRenderer::destinationSrsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CCipasMapRenderer::updateMap()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CCipasMapRenderer::mapUnitsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
