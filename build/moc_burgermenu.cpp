/****************************************************************************
** Meta object code from reading C++ file 'burgermenu.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../burgermenu.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'burgermenu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10BurgerMenuE_t {};
} // unnamed namespace

template <> constexpr inline auto BurgerMenu::qt_create_metaobjectdata<qt_meta_tag_ZN10BurgerMenuE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BurgerMenu",
        "iconChanged",
        "",
        "iconSizeChanged",
        "size",
        "menuWidthChanged",
        "width",
        "animatedChanged",
        "animated",
        "expandedChanged",
        "expanded",
        "triggered",
        "QAction*",
        "action",
        "addMenuAction",
        "label",
        "icon",
        "removeMenuAction",
        "setBurgerIcon",
        "setIconSize",
        "setMenuWidth",
        "setAnimated",
        "setExpanded",
        "iconSize",
        "menuWidth"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'iconChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'iconSizeChanged'
        QtMocHelpers::SignalData<void(const QSize &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QSize, 4 },
        }}),
        // Signal 'menuWidthChanged'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Signal 'animatedChanged'
        QtMocHelpers::SignalData<void(bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 8 },
        }}),
        // Signal 'expandedChanged'
        QtMocHelpers::SignalData<void(bool)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 10 },
        }}),
        // Signal 'triggered'
        QtMocHelpers::SignalData<void(QAction *)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
        // Slot 'addMenuAction'
        QtMocHelpers::SlotData<QAction *(QAction *)>(14, 2, QMC::AccessPublic, 0x80000000 | 12, {{
            { 0x80000000 | 12, 13 },
        }}),
        // Slot 'addMenuAction'
        QtMocHelpers::SlotData<QAction *(const QString &)>(14, 2, QMC::AccessPublic, 0x80000000 | 12, {{
            { QMetaType::QString, 15 },
        }}),
        // Slot 'addMenuAction'
        QtMocHelpers::SlotData<QAction *(const QIcon &, const QString &)>(14, 2, QMC::AccessPublic, 0x80000000 | 12, {{
            { QMetaType::QIcon, 16 }, { QMetaType::QString, 15 },
        }}),
        // Slot 'removeMenuAction'
        QtMocHelpers::SlotData<void(QAction *)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
        // Slot 'setBurgerIcon'
        QtMocHelpers::SlotData<void(const QIcon &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QIcon, 16 },
        }}),
        // Slot 'setIconSize'
        QtMocHelpers::SlotData<void(const QSize &)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QSize, 4 },
        }}),
        // Slot 'setMenuWidth'
        QtMocHelpers::SlotData<void(int)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'setAnimated'
        QtMocHelpers::SlotData<void(bool)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 8 },
        }}),
        // Slot 'setExpanded'
        QtMocHelpers::SlotData<void(bool)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 10 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'icon'
        QtMocHelpers::PropertyData<QIcon>(16, QMetaType::QIcon, QMC::DefaultPropertyFlags | QMC::Writable, 0),
        // property 'iconSize'
        QtMocHelpers::PropertyData<QSize>(23, QMetaType::QSize, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'menuWidth'
        QtMocHelpers::PropertyData<int>(24, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'animated'
        QtMocHelpers::PropertyData<bool>(8, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'expanded'
        QtMocHelpers::PropertyData<bool>(10, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 4),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BurgerMenu, qt_meta_tag_ZN10BurgerMenuE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject BurgerMenu::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10BurgerMenuE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10BurgerMenuE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10BurgerMenuE_t>.metaTypes,
    nullptr
} };

void BurgerMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BurgerMenu *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->iconChanged(); break;
        case 1: _t->iconSizeChanged((*reinterpret_cast< std::add_pointer_t<QSize>>(_a[1]))); break;
        case 2: _t->menuWidthChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->animatedChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->expandedChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->triggered((*reinterpret_cast< std::add_pointer_t<QAction*>>(_a[1]))); break;
        case 6: { QAction* _r = _t->addMenuAction((*reinterpret_cast< std::add_pointer_t<QAction*>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QAction**>(_a[0]) = std::move(_r); }  break;
        case 7: { QAction* _r = _t->addMenuAction((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QAction**>(_a[0]) = std::move(_r); }  break;
        case 8: { QAction* _r = _t->addMenuAction((*reinterpret_cast< std::add_pointer_t<QIcon>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])));
            if (_a[0]) *reinterpret_cast< QAction**>(_a[0]) = std::move(_r); }  break;
        case 9: _t->removeMenuAction((*reinterpret_cast< std::add_pointer_t<QAction*>>(_a[1]))); break;
        case 10: _t->setBurgerIcon((*reinterpret_cast< std::add_pointer_t<QIcon>>(_a[1]))); break;
        case 11: _t->setIconSize((*reinterpret_cast< std::add_pointer_t<QSize>>(_a[1]))); break;
        case 12: _t->setMenuWidth((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->setAnimated((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 14: _t->setExpanded((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAction* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAction* >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAction* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (BurgerMenu::*)()>(_a, &BurgerMenu::iconChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (BurgerMenu::*)(const QSize & )>(_a, &BurgerMenu::iconSizeChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (BurgerMenu::*)(int )>(_a, &BurgerMenu::menuWidthChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (BurgerMenu::*)(bool )>(_a, &BurgerMenu::animatedChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (BurgerMenu::*)(bool )>(_a, &BurgerMenu::expandedChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (BurgerMenu::*)(QAction * )>(_a, &BurgerMenu::triggered, 5))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QIcon*>(_v) = _t->burgerIcon(); break;
        case 1: *reinterpret_cast<QSize*>(_v) = _t->iconSize(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->menuWidth(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->animated(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->expanded(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setBurgerIcon(*reinterpret_cast<QIcon*>(_v)); break;
        case 1: _t->setIconSize(*reinterpret_cast<QSize*>(_v)); break;
        case 2: _t->setMenuWidth(*reinterpret_cast<int*>(_v)); break;
        case 3: _t->setAnimated(*reinterpret_cast<bool*>(_v)); break;
        case 4: _t->setExpanded(*reinterpret_cast<bool*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *BurgerMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BurgerMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10BurgerMenuE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int BurgerMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void BurgerMenu::iconChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BurgerMenu::iconSizeChanged(const QSize & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void BurgerMenu::menuWidthChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void BurgerMenu::animatedChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void BurgerMenu::expandedChanged(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void BurgerMenu::triggered(QAction * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}
QT_WARNING_POP
