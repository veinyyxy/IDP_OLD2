/****************************************************************************
** Meta object code from reading C++ file 'LegendWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../LegendWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LegendWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LegendWidget[] = {

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
      28,   14,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      63,   55,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LegendWidget[] = {
    "LegendWidget\0\0currentWidget\0"
    "SetCurrentWidget(QWidget*)\0visible\0"
    "setVisible(bool)\0"
};

void LegendWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LegendWidget *_t = static_cast<LegendWidget *>(_o);
        switch (_id) {
        case 0: _t->SetCurrentWidget((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 1: _t->setVisible((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LegendWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LegendWidget::staticMetaObject = {
    { &QTabWidget::staticMetaObject, qt_meta_stringdata_LegendWidget,
      qt_meta_data_LegendWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LegendWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LegendWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LegendWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LegendWidget))
        return static_cast<void*>(const_cast< LegendWidget*>(this));
    return QTabWidget::qt_metacast(_clname);
}

int LegendWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
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
void LegendWidget::SetCurrentWidget(QWidget * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
