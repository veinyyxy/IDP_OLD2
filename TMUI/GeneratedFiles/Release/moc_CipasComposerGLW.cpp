/****************************************************************************
** Meta object code from reading C++ file 'CipasComposerGLW.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasComposerGLW.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasComposerGLW.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCipasComposerGLW[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      50,   35,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      76,   72,   18,   18, 0x0a,
      97,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CCipasComposerGLW[] = {
    "CCipasComposerGLW\0\0extentChanged()\0"
    "fWidth,fHeight\0myImages(float,float)\0"
    "img\0updateImage(QImage*)\0updateFuckImage()\0"
};

void CCipasComposerGLW::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCipasComposerGLW *_t = static_cast<CCipasComposerGLW *>(_o);
        switch (_id) {
        case 0: _t->extentChanged(); break;
        case 1: _t->myImages((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 2: _t->updateImage((*reinterpret_cast< QImage*(*)>(_a[1]))); break;
        case 3: _t->updateFuckImage(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCipasComposerGLW::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCipasComposerGLW::staticMetaObject = {
    { &CComposerItem::staticMetaObject, qt_meta_stringdata_CCipasComposerGLW,
      qt_meta_data_CCipasComposerGLW, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCipasComposerGLW::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCipasComposerGLW::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCipasComposerGLW::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCipasComposerGLW))
        return static_cast<void*>(const_cast< CCipasComposerGLW*>(this));
    return CComposerItem::qt_metacast(_clname);
}

int CCipasComposerGLW::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CComposerItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CCipasComposerGLW::extentChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CCipasComposerGLW::myImages(float _t1, float _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
