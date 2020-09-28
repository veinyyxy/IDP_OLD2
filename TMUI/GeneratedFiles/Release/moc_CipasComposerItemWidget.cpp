/****************************************************************************
** Meta object code from reading C++ file 'CipasComposerItemWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasComposerItemWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasComposerItemWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCipasComposerItemWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   25,   25,   25, 0x0a,
      57,   25,   25,   25, 0x0a,
      93,   25,   25,   25, 0x0a,
     130,  128,   25,   25, 0x0a,
     181,  175,   25,   25, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CCipasComposerItemWidget[] = {
    "CCipasComposerItemWidget\0\0"
    "on_mFrameColorButton_clicked()\0"
    "on_mBackgroundColorButton_clicked()\0"
    "on_mOpacitySlider_sliderReleased()\0d\0"
    "on_mOutlineWidthSpinBox_valueChanged(double)\0"
    "state\0on_mFrameCheckBox_stateChanged(int)\0"
};

void CCipasComposerItemWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCipasComposerItemWidget *_t = static_cast<CCipasComposerItemWidget *>(_o);
        switch (_id) {
        case 0: _t->on_mFrameColorButton_clicked(); break;
        case 1: _t->on_mBackgroundColorButton_clicked(); break;
        case 2: _t->on_mOpacitySlider_sliderReleased(); break;
        case 3: _t->on_mOutlineWidthSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->on_mFrameCheckBox_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCipasComposerItemWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCipasComposerItemWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CCipasComposerItemWidget,
      qt_meta_data_CCipasComposerItemWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCipasComposerItemWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCipasComposerItemWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCipasComposerItemWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCipasComposerItemWidget))
        return static_cast<void*>(const_cast< CCipasComposerItemWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CCipasComposerItemWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
