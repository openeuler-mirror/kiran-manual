#pragma once
#include <QTreeWidget>
#include <QPainter>
#include <QRect>
#include <QModelIndex>

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

