#pragma once
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>

class TocTreeItemDelegate : public QStyledItemDelegate
{
public:
    explicit TocTreeItemDelegate(QObject* parent = nullptr);

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};