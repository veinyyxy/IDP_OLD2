/****************************************************************************
** Meta object code from reading C++ file 'CipasComposerView.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasComposerView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasComposerView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCipasComposerView[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: signature, parameters, type, tag, flags
      29,   20,   19,   19, 0x05,
      71,   65,   19,   19, 0x05,
     116,  112,   19,   19, 0x05,
     157,  153,   19,   19, 0x05,
     202,  194,   19,   19, 0x05,
     256,  247,   19,   19, 0x05,
     309,  303,   19,   19, 0x05,
     356,  350,   19,   19, 0x05,
     404,  397,   19,   19, 0x05,
     447,   19,   19,   19, 0x05,
     475,   19,   19,   19, 0x05,
     492,   19,   19,   19, 0x05,
     506,   19,   19,   19, 0x05,
     522,   19,   19,   19, 0x05,
     559,  539,   19,   19, 0x05,

 // slots: signature, parameters, type, tag, flags
     591,  586,   19,   19, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CCipasComposerView[] = {
    "CCipasComposerView\0\0selected\0"
    "selectedItemChanged(CComposerItem*)\0"
    "label\0composerLabelAdded(CCipasComposerLabel*)\0"
    "map\0composerMapAdded(CCipasComposerMap*)\0"
    "glw\0composerGLWAdded(CCipasComposerGLW*)\0"
    "picture\0composerPictureAdded(CCipasComposerPicture*)\0"
    "scalebar\0composerScaleBarAdded(CCipasComposerScaleBar*)\0"
    "shape\0composerShapeAdded(CCipasComposerShape*)\0"
    "arrow\0composerArrowAdded(CCipasComposerArrow*)\0"
    "legend\0composerLegendAdded(CCipasComposerLegend*)\0"
    "itemRemoved(CComposerItem*)\0"
    "actionFinished()\0emitMapFull()\0"
    "emitMapZoonIn()\0emitMapZoonOut()\0"
    "beginPoint,endPoint\0emitMapMove(QPoint,QPoint)\0"
    "item\0sendItemAddedSignal(CComposerItem*)\0"
};

void CCipasComposerView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCipasComposerView *_t = static_cast<CCipasComposerView *>(_o);
        switch (_id) {
        case 0: _t->selectedItemChanged((*reinterpret_cast< CComposerItem*(*)>(_a[1]))); break;
        case 1: _t->composerLabelAdded((*reinterpret_cast< CCipasComposerLabel*(*)>(_a[1]))); break;
        case 2: _t->composerMapAdded((*reinterpret_cast< CCipasComposerMap*(*)>(_a[1]))); break;
        case 3: _t->composerGLWAdded((*reinterpret_cast< CCipasComposerGLW*(*)>(_a[1]))); break;
        case 4: _t->composerPictureAdded((*reinterpret_cast< CCipasComposerPicture*(*)>(_a[1]))); break;
        case 5: _t->composerScaleBarAdded((*reinterpret_cast< CCipasComposerScaleBar*(*)>(_a[1]))); break;
        case 6: _t->composerShapeAdded((*reinterpret_cast< CCipasComposerShape*(*)>(_a[1]))); break;
        case 7: _t->composerArrowAdded((*reinterpret_cast< CCipasComposerArrow*(*)>(_a[1]))); break;
        case 8: _t->composerLegendAdded((*reinterpret_cast< CCipasComposerLegend*(*)>(_a[1]))); break;
        case 9: _t->itemRemoved((*reinterpret_cast< CComposerItem*(*)>(_a[1]))); break;
        case 10: _t->actionFinished(); break;
        case 11: _t->emitMapFull(); break;
        case 12: _t->emitMapZoonIn(); break;
        case 13: _t->emitMapZoonOut(); break;
        case 14: _t->emitMapMove((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< QPoint(*)>(_a[2]))); break;
        case 15: _t->sendItemAddedSignal((*reinterpret_cast< CComposerItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCipasComposerView::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCipasComposerView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_CCipasComposerView,
      qt_meta_data_CCipasComposerView, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCipasComposerView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCipasComposerView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCipasComposerView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCipasComposerView))
        return static_cast<void*>(const_cast< CCipasComposerView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int CCipasComposerView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void CCipasComposerView::selectedItemChanged(CComposerItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CCipasComposerView::composerLabelAdded(CCipasComposerLabel * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CCipasComposerView::composerMapAdded(CCipasComposerMap * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CCipasComposerView::composerGLWAdded(CCipasComposerGLW * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CCipasComposerView::composerPictureAdded(CCipasComposerPicture * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CCipasComposerView::composerScaleBarAdded(CCipasComposerScaleBar * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CCipasComposerView::composerShapeAdded(CCipasComposerShape * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CCipasComposerView::composerArrowAdded(CCipasComposerArrow * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CCipasComposerView::composerLegendAdded(CCipasComposerLegend * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CCipasComposerView::itemRemoved(CComposerItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CCipasComposerView::actionFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 10, 0);
}

// SIGNAL 11
void CCipasComposerView::emitMapFull()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}

// SIGNAL 12
void CCipasComposerView::emitMapZoonIn()
{
    QMetaObject::activate(this, &staticMetaObject, 12, 0);
}

// SIGNAL 13
void CCipasComposerView::emitMapZoonOut()
{
    QMetaObject::activate(this, &staticMetaObject, 13, 0);
}

// SIGNAL 14
void CCipasComposerView::emitMapMove(QPoint _t1, QPoint _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}
QT_END_MOC_NAMESPACE
