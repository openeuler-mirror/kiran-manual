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

#include "my-scroll-style.h"
MyScrollStyle::MyScrollStyle()
= default;

QRect MyScrollStyle::subControlRect(ComplexControl cc,
                                    const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget/* = nullptr*/) const
{
    if (cc == QStyle::CC_ScrollBar)
    {
        QRect ret;
        if (const auto* scrollbar = qstyleoption_cast<const QStyleOptionSlider*>(opt)) {
            const QRect scrollBarRect = scrollbar->rect;
            int sbextent = 0;
            int maxLen = ((scrollbar->orientation == Qt::Horizontal) ?
                          scrollBarRect.width() : scrollBarRect.height());
            int sliderLen;
            // 计算滑动长度
            if (scrollbar->maximum != scrollbar->minimum) {
                uint range = scrollbar->maximum - scrollbar->minimum;
                int contentLen = 0;
                sliderLen = 2 * maxLen / 3;
                if (widget)
                {
                    //Scrollbar设置的parent为：QAbstractScrollArea
                    auto* area = qobject_cast<QAbstractScrollArea*>(widget->parentWidget());
                    QWidget * w = area->viewport();
                    contentLen = w->height();
                    int nPageCount = contentLen / maxLen;
                    if (nPageCount > 0)
                    {
                        sliderLen = sliderLen / nPageCount;
                    }
                }
                int sliderMin = proxy()->pixelMetric(PM_ScrollBarSliderMin, scrollbar, widget);
                if (sliderLen < sliderMin || range > INT_MAX / 2)
                    sliderLen = sliderMin;
                if (sliderLen > maxLen)
                    sliderLen = maxLen;
            }
            else {
                sliderLen = maxLen;
            }

            int sliderStart = sbextent + sliderPositionFromValue(scrollbar->minimum,
                                                                 scrollbar->maximum,
                                                                 scrollbar->sliderPosition,
                                                                 maxLen - sliderLen,
                                                                 scrollbar->upsideDown);

            switch (sc) {
            // top/left button
            case SC_ScrollBarSubLine:
            // bottom/right button
            case SC_ScrollBarAddLine:
            // between top/left button and slider
            case SC_ScrollBarSubPage:
            // between bottom/right button and slider
            case SC_ScrollBarAddPage:
                break;
            case SC_ScrollBarGroove:
                if (scrollbar->orientation == Qt::Horizontal)
                    ret.setRect(sbextent, 0, scrollBarRect.width() - sbextent * 2,
                                scrollBarRect.height());
                else
                    ret.setRect(0, sbextent, scrollBarRect.width(),
                                scrollBarRect.height() - sbextent * 2);
                break;
            case SC_ScrollBarSlider:
                if (scrollbar->orientation == Qt::Horizontal)
                    ret.setRect(sliderStart, 0, sliderLen, scrollBarRect.height());
                else
                    ret.setRect(0, sliderStart, scrollBarRect.width(), sliderLen);
                break;
            default:
                break;
            }
            return ret;
        }
    }
    return QCommonStyle::subControlRect(cc, opt, sc, widget);
}
MyScrollStyle::~MyScrollStyle()
= default;
