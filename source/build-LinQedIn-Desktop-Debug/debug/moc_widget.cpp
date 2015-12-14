/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created: Sun 5. Jul 23:53:19 2015
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../c++/widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Widget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x0a,
      21,    7,    7,    7, 0x0a,
      35,    7,    7,    7, 0x0a,
      45,    7,    7,    7, 0x0a,
      57,    7,    7,    7, 0x0a,
      68,    7,    7,    7, 0x0a,
      78,    7,    7,    7, 0x0a,
      93,    7,    7,    7, 0x0a,
     101,    7,    7,    7, 0x0a,
     110,    7,    7,    7, 0x0a,
     124,  122,    7,    7, 0x0a,
     135,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Widget[] = {
    "Widget\0\0searchUser()\0adminSearch()\0"
    "removeQ()\0underline()\0redirect()\0"
    "goAbout()\0changeScreen()\0login()\0"
    "regAcc()\0changePsw()\0e\0error(int)\0"
    "changeType()\0"
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Widget *_t = static_cast<Widget *>(_o);
        switch (_id) {
        case 0: _t->searchUser(); break;
        case 1: _t->adminSearch(); break;
        case 2: _t->removeQ(); break;
        case 3: _t->underline(); break;
        case 4: _t->redirect(); break;
        case 5: _t->goAbout(); break;
        case 6: _t->changeScreen(); break;
        case 7: _t->login(); break;
        case 8: _t->regAcc(); break;
        case 9: _t->changePsw(); break;
        case 10: _t->error((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->changeType(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Widget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Widget::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_Widget,
      qt_meta_data_Widget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Widget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Widget))
        return static_cast<void*>(const_cast< Widget*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
