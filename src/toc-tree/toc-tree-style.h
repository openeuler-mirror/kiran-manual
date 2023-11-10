#pragma once
#include <QProxyStyle>

class QStyleOptionViewItem;
class TocTreeStyle : public QProxyStyle
{

Q_OBJECT
public:
    explicit TocTreeStyle(QStyle *style = nullptr);

    void drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const override;
    static QPainterPath roundedPath(const QStyleOptionViewItem *opt, const QWidget *w) ;
    void drawPanelItemViewRow(const QStyleOption *opt, QPainter *p, const QWidget *w) const;
    void drawPanelItemViewItem(const QStyleOption *opt, QPainter *p, const QWidget *w) const;
};
