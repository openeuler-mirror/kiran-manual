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

#include "toc-tree-item-delegate.h"

#include <QApplication>
#include <QPainter>
#include <QStyleOptionViewItemV4>
#include <QTreeView>

namespace Kiran
{
TocTreeItemDelegate::TocTreeItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

QSize TocTreeItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    int height = 36;  // 设置目录项的高度为50像素
    QSize size = QStyledItemDelegate::sizeHint(opt, index);
    size.setHeight(height);

    return size;
}
}  // namespace Kiran
