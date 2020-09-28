/****************************************************************************
** Meta object code from reading C++ file 'CipasTemplateTree.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CipasTemplateTree.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CipasTemplateTree.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CCipasTemplateTree[] = {

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
      32,   20,   19,   19, 0x0a,
      71,   19,   19,   19, 0x08,
      89,   19,   19,   19, 0x08,
     105,   19,   19,   19, 0x08,
     120,   19,   19,   19, 0x08,
     134,   19,   19,   19, 0x08,
     162,  153,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CCipasTemplateTree[] = {
    "CCipasTemplateTree\0\0item,column\0"
    "updateDomElement(QTreeWidgetItem*,int)\0"
    "rightMenu(QPoint)\0solt_newGroup()\0"
    "slot_newItem()\0slot_remove()\0"
    "slot_setDefaultT()\0cur,prev\0"
    "slot_currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)\0"
};

void CCipasTemplateTree::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CCipasTemplateTree *_t = static_cast<CCipasTemplateTree *>(_o);
        switch (_id) {
        case 0: _t->updateDomElement((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->rightMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 2: _t->solt_newGroup(); break;
        case 3: _t->slot_newItem(); break;
        case 4: _t->slot_remove(); break;
        case 5: _t->slot_setDefaultT(); break;
        case 6: _t->slot_currentItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< QTreeWidgetItem*(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CCipasTemplateTree::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CCipasTemplateTree::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_CCipasTemplateTree,
      qt_meta_data_CCipasTemplateTree, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CCipasTemplateTree::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CCipasTemplateTree::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CCipasTemplateTree::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCipasTemplateTree))
        return static_cast<void*>(const_cast< CCipasTemplateTree*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int CCipasTemplateTree::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
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
