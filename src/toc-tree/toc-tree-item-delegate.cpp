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

    int height = 45;  // 设置项的高度为50像素
    QSize size = QStyledItemDelegate::sizeHint(opt, index);
    size.setHeight(height);

    return size;
}
void TocTreeItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
//    QTextOption op;
//    op.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    QFont font;
//    font.setFamily("Microsoft YaHei");
//    font.setPixelSize(14);
//    painter->setFont(font);
//    //判断当前 item 是否选中
//    if (option.state & QStyle::State_Selected) {
//        //当前 item 被选中，高亮
////        painter->fillRect(option.rect, option.palette.highlight());
//        //自定义选中背景色
//        painter->fillRect(option.rect, QBrush(Qt::transparent));
//    }
//
//    painter->drawText(option.rect, index.data(Qt::DisplayRole).toString(), op);

//================================

    QStyledItemDelegate::paint(painter, option, index);
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    // 设置背景透明
//    opt.backgroundBrush = QBrush(Qt::transparent);

    // 绘制项
    painter->save();
    painter->setClipRect(opt.rect);
    QStyledItemDelegate::paint(painter, opt, index);
    painter->restore();
}
