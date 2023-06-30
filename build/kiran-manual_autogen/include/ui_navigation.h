/********************************************************************************
** Form generated from reading UI file 'navigation.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NAVIGATION_H
#define UI_NAVIGATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Navigation
{
public:

    void setupUi(QWidget *Navigation)
    {
        if (Navigation->objectName().isEmpty())
            Navigation->setObjectName(QString::fromUtf8("Navigation"));
        Navigation->resize(800, 600);

        retranslateUi(Navigation);

        QMetaObject::connectSlotsByName(Navigation);
    } // setupUi

    void retranslateUi(QWidget *Navigation)
    {
        Navigation->setWindowTitle(QCoreApplication::translate("Navigation", "Navigation", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Navigation: public Ui_Navigation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NAVIGATION_H
