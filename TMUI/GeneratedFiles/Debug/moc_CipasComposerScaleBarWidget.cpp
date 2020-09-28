/****************************************************************************
** Meta object code from reading C++ file 'CipasComposerScaleBarWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasComposerScaleBarWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasComposerScaleBarWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCipasComposerScaleBarWidget[] = {

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
      35,   30,   29,   29, 0x0a,
      72,   70,   29,   29, 0x0a,
     110,  108,   29,   29, 0x0a,
     152,  108,   29,   29, 0x0a,
     196,   70,   29,   29, 0x0a,
     238,   70,   29,   29, 0x0a,
     284,   30,   29,   29, 0x0a,
     327,  108,   29,   29, 0x0a,
     378,   29,   29,   29, 0x0a,
     408,   29,   29,   29, 0x0a,
     433,   30,   29,   29, 0x0a,
     480,  108,   29,   29, 0x0a,
     526,  108,   29,   29, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CCipasComposerScaleBarWidget[] = {
    "CCipasComposerScaleBarWidget\0\0text\0"
    "on_mMapComboBox_activated(QString)\0i\0"
    "on_mHeightSpinBox_valueChanged(int)\0"
    "d\0on_mLineWidthSpinBox_valueChanged(double)\0"
    "on_mSegmentSizeSpinBox_valueChanged(double)\0"
    "on_mSegmentsLeftSpinBox_valueChanged(int)\0"
    "on_mNumberOfSegmentsSpinBox_valueChanged(int)\0"
    "on_mUnitLabelLineEdit_textChanged(QString)\0"
    "on_mMapUnitsPerBarUnitSpinBox_valueChanged(double)\0"
    "on_mColorPushButton_clicked()\0"
    "on_mFontButton_clicked()\0"
    "on_mStyleComboBox_currentIndexChanged(QString)\0"
    "on_mLabelBarSpaceSpinBox_valueChanged(double)\0"
    "on_mBoxSizeSpinBox_valueChanged(double)\0"
};

void CCipasComposerScaleBarWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCipasComposerScaleBarWidget *_t = static_cast<CCipasComposerScaleBarWidget *>(_o);
        switch (_id) {
        case 0: _t->on_mMapComboBox_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->on_mHeightSpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_mLineWidthSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->on_mSegmentSizeSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->on_mSegmentsLeftSpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_mNumberOfSegmentsSpinBox_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_mUnitLabelLineEdit_textChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->on_mMapUnitsPerBarUnitSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->on_mColorPushButton_clicked(); break;
        case 9: _t->on_mFontButton_clicked(); break;
        case 10: _t->on_mStyleComboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->on_mLabelBarSpaceSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->on_mBoxSizeSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCipasComposerScaleBarWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCipasComposerScaleBarWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CCipasComposerScaleBarWidget,
      qt_meta_data_CCipasComposerScaleBarWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCipasComposerScaleBarWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCipasComposerScaleBarWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCipasComposerScaleBarWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCipasComposerScaleBarWidget))
        return static_cast<void*>(const_cast< CCipasComposerScaleBarWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CCipasComposerScaleBarWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
