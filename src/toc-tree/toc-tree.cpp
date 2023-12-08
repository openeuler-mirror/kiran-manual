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

#include "toc-tree.h"
#include "toc-tree-item-delegate.h"
#include "toc-tree-style.h"
namespace Kiran
{
TocTree::TocTree(QWidget *parent) : QTreeWidget(parent)
{
    this->init();
}

TocTree::~TocTree()
= default;

void TocTree::init()
{
    QPalette pll = this->palette();
    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
    this->setPalette(pll);

    this->setExpandsOnDoubleClick(false);
    this->setHeaderHidden(true);
    this->setFrameStyle(QFrame::NoFrame);
    this->setUniformRowHeights(true);
    this->setStyle(new TocTreeStyle());
    this->setItemDelegate(new TocTreeItemDelegate(this));
}
void TocTree::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    painter->fillRect(options.rect.x(), options.rect.y(), options.rect.width(), options.rect.height(), QColor(255,255,255,0));
    QTreeView::drawRow(painter, options, index);
}
}  // namespace Kiran
