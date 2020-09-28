/****************************************************************************
** Meta object code from reading C++ file 'CipasComposerArrowWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasComposerArrowWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasComposerArrowWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCipasComposerArrowWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   27,   26,   26, 0x08,
      74,   27,   26,   26, 0x08,
     121,   26,   26,   26, 0x08,
     160,  152,   26,   26, 0x08,
     203,  152,   26,   26, 0x08,
     241,   26,   26,   26, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CCipasComposerArrowWidget[] = {
    "CCipasComposerArrowWidget\0\0d\0"
    "on_mOutlineWidthSpinBox_valueChanged(double)\0"
    "on_mArrowHeadWidthSpinBox_valueChanged(double)\0"
    "on_mArrowColorButton_clicked()\0toggled\0"
    "on_mDefaultMarkerRadioButton_toggled(bool)\0"
    "on_mNoMarkerRadioButton_toggled(bool)\0"
    "setGuiElementValues()\0"
};

void CCipasComposerArrowWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCipasComposerArrowWidget *_t = static_cast<CCipasComposerArrowWidget *>(_o);
        switch (_id) {
        case 0: _t->on_mOutlineWidthSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->on_mArrowHeadWidthSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->on_mArrowColorButton_clicked(); break;
        case 3: _t->on_mDefaultMarkerRadioButton_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_mNoMarkerRadioButton_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setGuiElementValues(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCipasComposerArrowWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCipasComposerArrowWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CCipasComposerArrowWidget,
      qt_meta_data_CCipasComposerArrowWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCipasComposerArrowWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCipasComposerArrowWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCipasComposerArrowWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCipasComposerArrowWidget))
        return static_cast<void*>(const_cast< CCipasComposerArrowWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CCipasComposerArrowWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
