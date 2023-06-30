/****************************************************************************
** Meta object code from reading C++ file 'article.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/article.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'article.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Article_t {
    QByteArrayData data[13];
    char stringdata0[211];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Article_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Article_t qt_meta_stringdata_Article = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Article"
QT_MOC_LITERAL(1, 8, 8), // "backHome"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 3), // "key"
QT_MOC_LITERAL(4, 22, 31), // "on_treeWidget_itemDoubleClicked"
QT_MOC_LITERAL(5, 54, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(6, 71, 4), // "item"
QT_MOC_LITERAL(7, 76, 6), // "column"
QT_MOC_LITERAL(8, 83, 27), // "on_pushButtonSearch_clicked"
QT_MOC_LITERAL(9, 111, 29), // "on_pushButtonBackHome_clicked"
QT_MOC_LITERAL(10, 141, 32), // "on_textBrowser_backwardAvailable"
QT_MOC_LITERAL(11, 174, 4), // "arg1"
QT_MOC_LITERAL(12, 179, 31) // "on_textBrowser_forwardAvailable"

    },
    "Article\0backHome\0\0key\0"
    "on_treeWidget_itemDoubleClicked\0"
    "QTreeWidgetItem*\0item\0column\0"
    "on_pushButtonSearch_clicked\0"
    "on_pushButtonBackHome_clicked\0"
    "on_textBrowser_backwardAvailable\0arg1\0"
    "on_textBrowser_forwardAvailable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Article[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    2,   47,    2, 0x08 /* Private */,
       8,    0,   52,    2, 0x08 /* Private */,
       9,    0,   53,    2, 0x08 /* Private */,
      10,    1,   54,    2, 0x08 /* Private */,
      12,    1,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5, QMetaType::Int,    6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, QMetaType::Bool,   11,

       0        // eod
};

void Article::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Article *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->backHome((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->on_treeWidget_itemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->on_pushButtonSearch_clicked(); break;
        case 3: _t->on_pushButtonBackHome_clicked(); break;
        case 4: _t->on_textBrowser_backwardAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_textBrowser_forwardAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Article::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Article::backHome)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Article::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_Article.data,
    qt_meta_data_Article,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Article::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Article::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Article.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Article::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Article::backHome(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
