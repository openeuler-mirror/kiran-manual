#include "toc-tree-item-delegate.h"

#include <QApplication>
#include <QPainter>
#include <QStyleOptionViewItemV4>
#include <QTreeView>

TocTreeItemDelegate::TocTreeItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

QSize TocTreeItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    int height = 36;  // 设置项的高度为50像素
    QSize size = QStyledItemDelegate::sizeHint(opt, index);
    size.setHeight(height);

    return size;
}