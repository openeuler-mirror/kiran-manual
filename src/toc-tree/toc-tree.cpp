//
// Created by skyzcyou on 2023/11/10.
//

#include "toc-tree.h"
#include "toc-tree-item-delegate.h"
#include "toc-tree-style.h"
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
