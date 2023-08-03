//
// Created by skyzcyou on 2023/8/2.
//

#include "search-dialog.h"
#include <kiran-color-block.h>

SearchDialog::SearchDialog(QWidget *parent)
    : QDialog(parent)
{
    init();
}
SearchDialog::~SearchDialog()
= default;

void SearchDialog::setSearchText(const QString &text)
{
    this->m_searchText = text;
}


void SearchDialog::prevPosition()
{
    emit prevClicked();
}

void SearchDialog::nextPosition()
{
    emit nextClicked();
}
void SearchDialog::init()
{
    // 创建界面控件
    m_prevButton = new QPushButton(tr("Prev"), this);
    m_nextButton = new QPushButton(tr("Next"), this);
//    m_countLabel = new QLabel(tr("count: "),this);
    m_closeButton = new QPushButton(tr("Close"), this);

    // 设置按钮样式
//    m_closeButton->setFixedSize(20, 20);
//    m_closeButton->setStyleSheet("QPushButton {"
//                                 "background: transparent;"
//                                 "border: none;"
//                                 "color: white;"
//                                 "font-size: 16px;"
//                                 "}"
//                                 "QPushButton:hover {"
//                                 "background: #F44336;"
//                                 "}");

    // 设置布局
    auto* dialogLayout = new QHBoxLayout(this);
    dialogLayout->setMargin(0);
    auto* contentWidget = new KiranColorBlock(this);
    dialogLayout->addWidget(contentWidget);
    auto* hLayout = new QHBoxLayout(contentWidget);
    hLayout->addWidget(m_prevButton);
    hLayout->addWidget(m_nextButton);
    hLayout->addWidget(m_closeButton);
//    auto* vLayout = new QVBoxLayout(this);
//    vLayout->addLayout(hLayout);
//    vLayout->addWidget(m_countLabel);

    // 连接槽函数
    connect(m_prevButton, &QPushButton::clicked, this, &SearchDialog::prevPosition);
    connect(m_nextButton, &QPushButton::clicked, this, &SearchDialog::nextPosition);
    connect(m_closeButton, &QPushButton::clicked, this, &SearchDialog::close);

}
