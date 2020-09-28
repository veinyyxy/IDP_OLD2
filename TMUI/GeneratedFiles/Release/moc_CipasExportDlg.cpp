/****************************************************************************
** Meta object code from reading C++ file 'CipasExportDlg.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasExportDlg.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasExportDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CipasExportDlg[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      44,   15,   15,   15, 0x0a,
      70,   15,   15,   15, 0x0a,
     104,   99,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CipasExportDlg[] = {
    "CipasExportDlg\0\0on_pushButton_dlg_clicked()\0"
    "on_okpushButton_clicked()\0"
    "on_spinBox_valueChanged(int)\0text\0"
    "on_lineEdit_sp_textChanged(QString)\0"
};

void CipasExportDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CipasExportDlg *_t = static_cast<CipasExportDlg *>(_o);
        switch (_id) {
        case 0: _t->on_pushButton_dlg_clicked(); break;
        case 1: _t->on_okpushButton_clicked(); break;
        case 2: _t->on_spinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_lineEdit_sp_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CipasExportDlg::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CipasExportDlg::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CipasExportDlg,
      qt_meta_data_CipasExportDlg, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CipasExportDlg::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CipasExportDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CipasExportDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CipasExportDlg))
        return static_cast<void*>(const_cast< CipasExportDlg*>(this));
    return QDialog::qt_metacast(_clname);
}

int CipasExportDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
