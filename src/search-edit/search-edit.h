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

#pragma once
#include <kiran-search-box.h>
#include "search-dialog.h"
/**
 * SearchEdit - 搜索框
 * */
class SearchEdit : public KiranSearchBox
{
    Q_OBJECT
public:
    explicit SearchEdit(QWidget* parent = nullptr);
    ~SearchEdit() override;
    void setSearchField(const QString& searchField);

public slots:
    // 作出搜索行为，内部会根据当前的搜索域发出不同的搜索请求
    void doSearch();
    void updateSearchCount(int count, int index);

signals:
    // 发起搜索文本浏览器请求
    void requestSearchTextBrowserNext(const QString& keyword);
    void requestSearchTextBrowserPrev(const QString& keyword);
    void requestSearchTextBrowserClosed(const QString& keyword);
    // 发起搜索导航页菜单请求
    void requestSearchNavItem(const QString& keyword);
    // 文本改变信号
    void requestSearchKeywordChanged(const QString& keyword);

protected:
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;

private:
    void init();
    void initSearchDialog();

    // 搜索工具框： 提供搜索时的便捷跳转功能
    SearchDialog* m_searchDialog{};
    // 搜索域: 页面 StackWidget 切换时监听更改
    QString m_searchField;
};