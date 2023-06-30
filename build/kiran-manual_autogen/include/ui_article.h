/********************************************************************************
** Form generated from reading UI file 'article.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ARTICLE_H
#define UI_ARTICLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Article
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonBackHome;
    QPushButton *pushButtonBackward;
    QPushButton *pushButtonForward;
    QSpacerItem *horizontalSpacer;
    QLineEdit *lineEditKeyword;
    QPushButton *pushButtonSearch;
    QHBoxLayout *horizontalLayout_4;
    QTreeWidget *treeWidget;
    QTextBrowser *textBrowser;
    QMenuBar *menubar;

    void setupUi(QWidget *Article)
    {
        if (Article->objectName().isEmpty())
            Article->setObjectName(QString::fromUtf8("Article"));
        Article->resize(800, 600);
        verticalLayout_2 = new QVBoxLayout(Article);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonBackHome = new QPushButton(Article);
        pushButtonBackHome->setObjectName(QString::fromUtf8("pushButtonBackHome"));

        horizontalLayout->addWidget(pushButtonBackHome);

        pushButtonBackward = new QPushButton(Article);
        pushButtonBackward->setObjectName(QString::fromUtf8("pushButtonBackward"));
        pushButtonBackward->setEnabled(true);

        horizontalLayout->addWidget(pushButtonBackward);

        pushButtonForward = new QPushButton(Article);
        pushButtonForward->setObjectName(QString::fromUtf8("pushButtonForward"));

        horizontalLayout->addWidget(pushButtonForward);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        lineEditKeyword = new QLineEdit(Article);
        lineEditKeyword->setObjectName(QString::fromUtf8("lineEditKeyword"));

        horizontalLayout->addWidget(lineEditKeyword);

        pushButtonSearch = new QPushButton(Article);
        pushButtonSearch->setObjectName(QString::fromUtf8("pushButtonSearch"));

        horizontalLayout->addWidget(pushButtonSearch);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        treeWidget = new QTreeWidget(Article);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("\346\226\207\346\241\243\347\233\256\345\275\225"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(treeWidget->sizePolicy().hasHeightForWidth());
        treeWidget->setSizePolicy(sizePolicy);

        horizontalLayout_4->addWidget(treeWidget);

        textBrowser = new QTextBrowser(Article);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(3);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(textBrowser->sizePolicy().hasHeightForWidth());
        textBrowser->setSizePolicy(sizePolicy1);

        horizontalLayout_4->addWidget(textBrowser);


        verticalLayout->addLayout(horizontalLayout_4);


        verticalLayout_2->addLayout(verticalLayout);

        menubar = new QMenuBar(Article);
        menubar->setObjectName(QString::fromUtf8("menubar"));

        verticalLayout_2->addWidget(menubar);


        retranslateUi(Article);

        QMetaObject::connectSlotsByName(Article);
    } // setupUi

    void retranslateUi(QWidget *Article)
    {
        Article->setWindowTitle(QCoreApplication::translate("Article", "Article", nullptr));
        pushButtonBackHome->setText(QCoreApplication::translate("Article", "HOME", nullptr));
        pushButtonBackward->setText(QCoreApplication::translate("Article", "<-", nullptr));
        pushButtonForward->setText(QCoreApplication::translate("Article", "->", nullptr));
        pushButtonSearch->setText(QCoreApplication::translate("Article", "Search", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Article: public Ui_Article {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ARTICLE_H
