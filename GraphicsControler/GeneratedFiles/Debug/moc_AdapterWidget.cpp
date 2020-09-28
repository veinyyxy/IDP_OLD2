/****************************************************************************
** Meta object code from reading C++ file 'AdapterWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../AdapterWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AdapterWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ViewerQT[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   10,    9,    9, 0x05,
      58,   51,    9,    9, 0x05,
      82,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
     101,   93,    9,    9, 0x0a,
     118,    9,    9,    9, 0x0a,
     147,  132,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ViewerQT[] = {
    "ViewerQT\0\0currentWidget\0"
    "SetCurrentWidget(QWidget*)\0pImage\0"
    "PrintImage(osg::Image*)\0toUpdate()\0"
    "visible\0setVisible(bool)\0Slot_Notify()\0"
    "fWidth,fHeight\0GiveYouFuckImage(float,float)\0"
};

void ViewerQT::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ViewerQT *_t = static_cast<ViewerQT *>(_o);
        switch (_id) {
        case 0: _t->SetCurrentWidget((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 1: _t->PrintImage((*reinterpret_cast< osg::Image*(*)>(_a[1]))); break;
        case 2: _t->toUpdate(); break;
        case 3: _t->setVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->Slot_Notify(); break;
        case 5: _t->GiveYouFuckImage((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ViewerQT::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ViewerQT::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ViewerQT,
      qt_meta_data_ViewerQT, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ViewerQT::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ViewerQT::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ViewerQT::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ViewerQT))
        return static_cast<void*>(const_cast< ViewerQT*>(this));
    return QWidget::qt_metacast(_clname);
}

int ViewerQT::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void ViewerQT::SetCurrentWidget(QWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ViewerQT::PrintImage(osg::Image * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ViewerQT::toUpdate()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
