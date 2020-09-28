/****************************************************************************
** Meta object code from reading C++ file 'CipasComposerShapeWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasComposerShapeWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasComposerShapeWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCipasComposerShapeWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      32,   27,   26,   26, 0x08,
      79,   26,   26,   26, 0x08,
     114,  112,   26,   26, 0x08,
     165,  159,   26,   26, 0x08,
     207,   26,   26,   26, 0x08,
     241,  237,   26,   26, 0x08,
     279,   26,   26,   26, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CCipasComposerShapeWidget[] = {
    "CCipasComposerShapeWidget\0\0text\0"
    "on_mShapeComboBox_currentIndexChanged(QString)\0"
    "on_mOutlineColorButton_clicked()\0d\0"
    "on_mOutlineWidthSpinBox_valueChanged(double)\0"
    "state\0on_mTransparentCheckBox_stateChanged(int)\0"
    "on_mFillColorButton_clicked()\0val\0"
    "on_mRotationSpinBox_valueChanged(int)\0"
    "setGuiElementValues()\0"
};

void CCipasComposerShapeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCipasComposerShapeWidget *_t = static_cast<CCipasComposerShapeWidget *>(_o);
        switch (_id) {
        case 0: _t->on_mShapeComboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->on_mOutlineColorButton_clicked(); break;
        case 2: _t->on_mOutlineWidthSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->on_mTransparentCheckBox_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_mFillColorButton_clicked(); break;
        case 5: _t->on_mRotationSpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setGuiElementValues(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCipasComposerShapeWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCipasComposerShapeWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CCipasComposerShapeWidget,
      qt_meta_data_CCipasComposerShapeWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCipasComposerShapeWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCipasComposerShapeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCipasComposerShapeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCipasComposerShapeWidget))
        return static_cast<void*>(const_cast< CCipasComposerShapeWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CCipasComposerShapeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
