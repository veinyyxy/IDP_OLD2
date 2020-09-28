/****************************************************************************
** Meta object code from reading C++ file 'CipasComposerPicture.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasComposerPicture.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasComposerPicture.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCipasComposerPicture[] = {

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
      25,   23,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CCipasComposerPicture[] = {
    "CCipasComposerPicture\0\0r\0setRotation(double)\0"
};

void CCipasComposerPicture::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCipasComposerPicture *_t = static_cast<CCipasComposerPicture *>(_o);
        switch (_id) {
        case 0: _t->setRotation((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCipasComposerPicture::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCipasComposerPicture::staticMetaObject = {
    { &CComposerItem::staticMetaObject, qt_meta_stringdata_CCipasComposerPicture,
      qt_meta_data_CCipasComposerPicture, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCipasComposerPicture::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCipasComposerPicture::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCipasComposerPicture::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCipasComposerPicture))
        return static_cast<void*>(const_cast< CCipasComposerPicture*>(this));
    return CComposerItem::qt_metacast(_clname);
}

int CCipasComposerPicture::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CComposerItem::qt_metacall(_c, _id, _a);
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
