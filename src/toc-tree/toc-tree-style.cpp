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

#include "toc-tree-style.h"

#include <QStyleOption>
#include <QPainter>
#include <QPainterPath>

namespace Kiran
{
TocTreeStyle::TocTreeStyle(QStyle *style) : QProxyStyle(style)
{
}

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

// 绘制圆角背景
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
            p->fillRect(vopt->rect, vopt->palette.brush(cg, QPalette::Highlight)); // 选中高亮
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

int TocTreeStyle::pixelMetric(QStyle::PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
        switch (metric) {
        case PM_FocusFrameHMargin:
            return 12;
        default:
            break;
        }
        return QProxyStyle::pixelMetric(metric, option, widget);
}
}  // namespace Kiran