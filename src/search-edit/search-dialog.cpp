/**
 * Copyright (c) 2020 ~ 2024 KylinSec Co., Ltd.
 * kiran-manual is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 *
 * Author:     youzhengcai <youzhengcai@kylinsec.com.cn>
 */

#include "search-dialog.h"
#include <kiran-color-block.h>

SearchDialog::SearchDialog(QWidget* parent)
    : QDialog(parent)
{
    init();
}

SearchDialog::~SearchDialog() = default;

void SearchDialog::setSearchText(const QString& text)
{
    this->m_searchText = text;
}

void SearchDialog::setMatchCount(int count, int index)
{
    m_countLabel->setText(QString::number(index) + "/" + QString::number(count));
}

void SearchDialog::init()
{
    // 创建界面控件
    m_prevButton = new QPushButton(tr("Prev"), this);
    m_nextButton = new QPushButton(tr("Next"), this);
    m_countLabel = new QLabel(tr("Count: "), this);
    m_closeButton = new QPushButton(tr("Close"), this);

    // 设置布局
    auto dialogLayout = new QHBoxLayout(this);
    dialogLayout->setMargin(0);
    auto btnToolsWidget = new KiranColorBlock(this);
    auto btnToolsLayout = new QHBoxLayout(btnToolsWidget);
    btnToolsLayout->addWidget(m_countLabel);
    btnToolsLayout->addWidget(m_prevButton);
    btnToolsLayout->addWidget(m_nextButton);
    btnToolsLayout->addWidget(m_closeButton);
    dialogLayout->addWidget(btnToolsWidget);

    // 点击按钮发送 searchDialog 信号
    // clang-format off
    connect(m_prevButton, &QPushButton::clicked, [this]() {
        emit sdPrevClicked();
    });
    connect(m_nextButton, &QPushButton::clicked, [this]() {
        emit sdNextClicked();
    });
    connect(m_closeButton, &QPushButton::clicked, [this]() {
        this->close();
        emit sdCloseClicked();
    });
    // clang-format on
}

