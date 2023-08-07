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

#include "search-edit.h"
#include "constants.h"
#include "search-edit/search-dialog.h"

#include <kiran-log/qt5-log-i.h>
#include <kiran-message-box.h>
#include <QEvent>
#include <QKeyEvent>

SearchEdit::SearchEdit(QWidget *parent)
    : KiranSearchBox(parent)
{
    init();
}

SearchEdit::~SearchEdit() = default;

void SearchEdit::init()
{
    setPlaceholderText(tr("Enter keywords to search"));
    setClearButtonEnabled(true);
    // 绑定搜索框回车事件
    connect(this, &QLineEdit::returnPressed, this, &SearchEdit::doSearch);
}

void SearchEdit::initSearchDialog()
{
    m_searchDialog = new SearchDialog(this);
    m_searchDialog->setWindowTitle(tr("Search Keyword"));
    m_searchDialog->setFixedWidth(this->width());
    m_searchDialog->setSearchText(this->text());

    // 取消标题栏
    //    m_searchDialog->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    QPoint pos = mapToGlobal(QPoint(0, height())) + QPoint(0, 40);
    //    QPoint pos = mapToGlobal(QPoint(0, height()));
    m_searchDialog->move(pos);

    // clang-format off
    connect(m_searchDialog, &SearchDialog::sdNextClicked, [this]() {
        emit requestSearchTextBrowserNext(this->text());
    });

    connect(m_searchDialog, &SearchDialog::sdPrevClicked, [this]() {
        emit requestSearchTextBrowserPrev(this->text());
    });

    connect(m_searchDialog, &SearchDialog::sdCloseClicked, [this]() {
        emit requestSearchTextBrowserClosed(this->text());
    });

    connect(this, &SearchEdit::textChanged, [this]() {
        emit requestSearchKeywordChanged(this->text());
    });
    // clang-format on
}
void SearchEdit::doSearch()
{
    // 作出搜索行为时，初始化并弹出搜索工具框
    // TODO: 初始化和弹出分离, 先初始化完毕，按需弹出。现在遇到的问题是会导致位置不正确
    this->initSearchDialog();
    m_searchDialog->show();
    // 利用 StackWidget 中当前激活到页面 Object name 来判定搜索域
    if (m_searchField == NAVIGATION_OBJECT_NAME)
    {
        emit requestSearchNavItem(this->text());
    }
    else if (m_searchField == DOCUMENT_OBJECT_NAME)
    {
        emit requestSearchTextBrowserNext(this->text());
    }
}

void SearchEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
}
void SearchEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
}
void SearchEdit::setSearchFiled(const QString &searchField)
{
    this->m_searchField = searchField;
}
void SearchEdit::updateSearchCount(int count, int index)
{
    this->m_searchDialog->setMatchCount(count, index);
}
