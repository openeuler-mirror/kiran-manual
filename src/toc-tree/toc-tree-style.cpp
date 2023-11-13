//
// Created by skyzcyou on 2023/11/10.
//

#include "toc-tree-style.h"

#include <QStyleOption>
#include <QPainter>
#include <QPainterPath>

TocTreeStyle::TocTreeStyle(QStyle *style) : QProxyStyle(style)
{
}

// 重载绘制函数
void TocTreeStyle::drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{

    switch (pe) {
    case PE_PanelItemViewRow:
//        drawPanelItemViewRow(opt, p, w);
        break;
    case PE_PanelItemViewItem:
        drawPanelItemViewItem(opt, p, w);
        break;
    default:
        QProxyStyle::drawPrimitive(pe, opt, p, w);
        break;
    }
}

// 绘制路径计算函数
// 计算绘制路径即选中状态、行背景所需绘制区域。
QPainterPath TocTreeStyle::roundedPath(const QStyleOptionViewItem *o, const QWidget *w)
{
    int radius = 6;
    int backgroundMarginLeft = 0;
    int backgroundMarginRight = 24;

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(o->rect.adjusted(backgroundMarginLeft, 0, -backgroundMarginRight, 0), radius, radius);
    return path;
}

// 绘制选中状态
void TocTreeStyle::drawPanelItemViewRow(const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
    p->save();
    if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem *>(opt)) {

        p->setRenderHint(QPainter::Antialiasing);
        QPainterPath path = roundedPath(vopt, w);
        QPalette::ColorGroup cg = (w ? w->isEnabled() : (vopt->state & QStyle::State_Enabled))
                                  ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(vopt->state & QStyle::State_Active))
            cg = QPalette::Inactive;

        if ((vopt->state & QStyle::State_Selected) &&  proxy()->styleHint(QStyle::SH_ItemView_ShowDecorationSelected, opt, w))
            p->fillRect(vopt->rect, vopt->palette.brush(cg, QPalette::Highlight));
        else if (vopt->features & QStyleOptionViewItem::Alternate)
            p->fillPath(path, vopt->palette.brush(cg, QPalette::AlternateBase));
    }
    p->restore();
}
// 绘制行背景
// 行背景即列表背景，通常情况下没有绘制。一般仅在设置了行颜色交替的情况下绘制。
void TocTreeStyle::drawPanelItemViewItem(const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
    p->save();
    if (const auto vopt = qstyleoption_cast<const QStyleOptionViewItem *>(opt)) {

        p->setRenderHint(QPainter::Antialiasing);
        QPainterPath path = roundedPath(vopt, w);
        QPalette::ColorGroup cg = (w ? w->isEnabled() : (vopt->state & QStyle::State_Enabled))
                                  ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(vopt->state & QStyle::State_Active))
            cg = QPalette::Inactive;
        if (vopt->showDecorationSelected && (vopt->state & QStyle::State_Selected)) {

            p->fillPath(path, vopt->palette.brush(cg, QPalette::Highlight));
        } else {

            if (vopt->backgroundBrush.style() != Qt::NoBrush) {

                QPointF oldBO = p->brushOrigin();
                p->setBrushOrigin(vopt->rect.topLeft());
                p->fillPath(path, vopt->backgroundBrush);
                p->setBrushOrigin(oldBO);
            }

            if (vopt->state & QStyle::State_Selected) {
                QRect textRect = subElementRect(QStyle::SE_ItemViewItemText,  opt, w);
                p->fillRect(textRect, vopt->palette.brush(cg, QPalette::Highlight));
            }
        }
    }
    p->restore();
}