/****************************************************************************
** Meta object code from reading C++ file 'CipasCompositionWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasCompositionWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasCompositionWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCipasCompositionWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   25,   24,   24, 0x0a,
      81,   25,   24,   24, 0x0a,
     133,   25,   24,   24, 0x0a,
     191,   24,   24,   24, 0x0a,
     237,   24,   24,   24, 0x0a,
     290,  284,   24,   24, 0x0a,
     336,  330,   24,   24, 0x0a,
     380,  330,   24,   24, 0x0a,
     423,  421,   24,   24, 0x0a,
     470,  421,   24,   24, 0x0a,
     510,  421,   24,   24, 0x0a,
     550,   24,   24,   24, 0x0a,
     580,   25,   24,   24, 0x0a,
     631,  421,   24,   24, 0x0a,
     672,   24,   24,   24, 0x0a,
     704,   24,   24,   24, 0x0a,
     731,   25,   24,   24, 0x0a,
     775,   24,   24,   24, 0x0a,
     806,   24,   24,   24, 0x0a,
     842,   24,   24,   24, 0x0a,
     875,  330,   24,   24, 0x0a,
     913,  421,   24,   24, 0x0a,
     953,  421,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CCipasCompositionWidget[] = {
    "CCipasCompositionWidget\0\0text\0"
    "on_mPaperSizeComboBox_currentIndexChanged(QString)\0"
    "on_mPaperUnitsComboBox_currentIndexChanged(QString)\0"
    "on_mPaperOrientationComboBox_currentIndexChanged(QString)\0"
    "on_mPaperWidthDoubleSpinBox_editingFinished()\0"
    "on_mPaperHeightDoubleSpinBox_editingFinished()\0"
    "value\0on_mResolutionSpinBox_valueChanged(int)\0"
    "state\0on_mPrintAsRasterCheckBox_stateChanged(int)\0"
    "on_mSnapToGridCheckBox_stateChanged(int)\0"
    "d\0on_mGridResolutionSpinBox_valueChanged(double)\0"
    "on_mOffsetXSpinBox_valueChanged(double)\0"
    "on_mOffsetYSpinBox_valueChanged(double)\0"
    "on_mGridColorButton_clicked()\0"
    "on_mGridStyleComboBox_currentIndexChanged(QString)\0"
    "on_mPenWidthSpinBox_valueChanged(double)\0"
    "displayCompositionWidthHeight()\0"
    "on_mpColorButton_clicked()\0"
    "on_mpLineStyle_currentIndexChanged(QString)\0"
    "on_mpLineNum_editingFinished()\0"
    "on_mpLineInterval_editingFinished()\0"
    "on_mpLineWidth_editingFinished()\0"
    "on_mpIsDrawLineRect_stateChanged(int)\0"
    "on_mMarginXSpinBox_valueChanged(double)\0"
    "on_mMarginYSpinBox_valueChanged(double)\0"
};

void CCipasCompositionWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCipasCompositionWidget *_t = static_cast<CCipasCompositionWidget *>(_o);
        switch (_id) {
        case 0: _t->on_mPaperSizeComboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->on_mPaperUnitsComboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->on_mPaperOrientationComboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->on_mPaperWidthDoubleSpinBox_editingFinished(); break;
        case 4: _t->on_mPaperHeightDoubleSpinBox_editingFinished(); break;
        case 5: _t->on_mResolutionSpinBox_valueChanged((*reinterpret_cast< const int(*)>(_a[1]))); break;
        case 6: _t->on_mPrintAsRasterCheckBox_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_mSnapToGridCheckBox_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_mGridResolutionSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->on_mOffsetXSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->on_mOffsetYSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->on_mGridColorButton_clicked(); break;
        case 12: _t->on_mGridStyleComboBox_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->on_mPenWidthSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 14: _t->displayCompositionWidthHeight(); break;
        case 15: _t->on_mpColorButton_clicked(); break;
        case 16: _t->on_mpLineStyle_currentIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->on_mpLineNum_editingFinished(); break;
        case 18: _t->on_mpLineInterval_editingFinished(); break;
        case 19: _t->on_mpLineWidth_editingFinished(); break;
        case 20: _t->on_mpIsDrawLineRect_stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->on_mMarginXSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 22: _t->on_mMarginYSpinBox_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCipasCompositionWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCipasCompositionWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CCipasCompositionWidget,
      qt_meta_data_CCipasCompositionWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCipasCompositionWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCipasCompositionWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCipasCompositionWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCipasCompositionWidget))
        return static_cast<void*>(const_cast< CCipasCompositionWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CCipasCompositionWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
