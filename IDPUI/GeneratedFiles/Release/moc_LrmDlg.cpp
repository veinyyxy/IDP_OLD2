/****************************************************************************
** Meta object code from reading C++ file 'LrmDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../LrmDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LrmDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LrmDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      17,    7,    7,    7, 0x0a,
      32,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LrmDlg[] = {
    "LrmDlg\0\0OkDraw()\0LrmConfig(int)\0"
    "OkClicked()\0"
};

void LrmDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LrmDlg *_t = static_cast<LrmDlg *>(_o);
        switch (_id) {
        case 0: _t->OkDraw(); break;
        case 1: _t->LrmConfig((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->OkClicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LrmDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LrmDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_LrmDlg,
      qt_meta_data_LrmDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LrmDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LrmDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LrmDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LrmDlg))
        return static_cast<void*>(const_cast< LrmDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int LrmDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void LrmDlg::OkDraw()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
