/****************************************************************************
** Meta object code from reading C++ file 'CipasComposerLabelWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasComposerLabelWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasComposerLabelWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCipasComposerLabelWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      27,   26,   26,   26, 0x0a,
      54,   26,   26,   26, 0x0a,
      81,   79,   26,   26, 0x0a,
     126,   26,   26,   26, 0x0a,
     156,   26,   26,   26, 0x0a,
     188,   26,   26,   26, 0x0a,
     218,   26,   26,   26, 0x0a,
     249,   26,   26,   26, 0x0a,
     278,   26,   26,   26, 0x0a,
     310,   26,   26,   26, 0x0a,
     347,  342,   26,   26, 0x0a,
     394,  388,   26,   26, 0x0a,
     435,   26,   26,   26, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CCipasComposerLabelWidget[] = {
    "CCipasComposerLabelWidget\0\0"
    "on_mTextEdit_textChanged()\0"
    "on_mFontButton_clicked()\0d\0"
    "on_mMarginDoubleSpinBox_valueChanged(double)\0"
    "on_mFontColorButton_clicked()\0"
    "on_mCenterRadioButton_clicked()\0"
    "on_mLeftRadioButton_clicked()\0"
    "on_mRightRadioButton_clicked()\0"
    "on_mTopRadioButton_clicked()\0"
    "on_mBottomRadioButton_clicked()\0"
    "on_mMiddleRadioButton_clicked()\0text\0"
    "on_mLabelIdLineEdit_textChanged(QString)\0"
    "state\0on_advanceLabeCheckBox_stateChanged(int)\0"
    "setGuiElementValues()\0"
};

void CCipasComposerLabelWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCipasComposerLabelWidget *_t = static_cast<CCipasComposerLabelWidget *>(_o);
        switch (_id) {
        case 0: _t->on_mTextEdit_textChanged(); break;
        case 1: _t->on_mFontButton_clicked(); break;
        case 2: _t->on_mMarginDoubleSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->on_mFontColorButton_clicked(); break;
        case 4: _t->on_mCenterRadioButton_clicked(); break;
        case 5: _t->on_mLeftRadioButton_clicked(); break;
        case 6: _t->on_mRightRadioButton_clicked(); break;
        case 7: _t->on_mTopRadioButton_clicked(); break;
        case 8: _t->on_mBottomRadioButton_clicked(); break;
        case 9: _t->on_mMiddleRadioButton_clicked(); break;
        case 10: _t->on_mLabelIdLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->on_advanceLabeCheckBox_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->setGuiElementValues(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCipasComposerLabelWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCipasComposerLabelWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CCipasComposerLabelWidget,
      qt_meta_data_CCipasComposerLabelWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCipasComposerLabelWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCipasComposerLabelWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCipasComposerLabelWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCipasComposerLabelWidget))
        return static_cast<void*>(const_cast< CCipasComposerLabelWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CCipasComposerLabelWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
