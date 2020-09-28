/****************************************************************************
** Meta object code from reading C++ file 'Draw3dPieChart.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Draw3dPieChart.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Draw3dPieChart.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CDraw3dPieChart[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CDraw3dPieChart[] = {
    "CDraw3dPieChart\0\0closewindow()\0"
};

void CDraw3dPieChart::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CDraw3dPieChart *_t = static_cast<CDraw3dPieChart *>(_o);
        switch (_id) {
        case 0: _t->closewindow(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CDraw3dPieChart::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CDraw3dPieChart::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CDraw3dPieChart,
      qt_meta_data_CDraw3dPieChart, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CDraw3dPieChart::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CDraw3dPieChart::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CDraw3dPieChart::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CDraw3dPieChart))
        return static_cast<void*>(const_cast< CDraw3dPieChart*>(this));
    return QWidget::qt_metacast(_clname);
}

int CDraw3dPieChart::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CDraw3dPieChart::closewindow()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
