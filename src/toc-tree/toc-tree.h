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
#include <QTreeWidget>
#include <QPainter>
#include <QRect>
#include <QModelIndex>

namespace Kiran
{
class TocTree : public QTreeWidget
{
Q_OBJECT

public:
    explicit TocTree(QWidget * parent = nullptr);
    ~TocTree() override;
    void drawBranches(QPainter* painter,const QRect& rect,const QModelIndex& index) const override {;};
    void drawRow(QPainter *painter , const QStyleOptionViewItem &options, const QModelIndex &index) const override;

private:
    void init();
};
}  // namespace Kiran

