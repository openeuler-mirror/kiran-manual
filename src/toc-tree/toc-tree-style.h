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

#pragma once
#include <QProxyStyle>

class QStyleOptionViewItem;

namespace Kiran
{
class TocTreeStyle : public QProxyStyle
{

Q_OBJECT
public:
    explicit TocTreeStyle(QStyle *style = nullptr);

    void drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const override;
    static QPainterPath roundedPath(const QStyleOptionViewItem *opt, const QWidget *w) ;
    void drawPanelItemViewRow(const QStyleOption *opt, QPainter *p, const QWidget *w) const;
    void drawPanelItemViewItem(const QStyleOption *opt, QPainter *p, const QWidget *w) const;
    // 设置 QTreeWidgetItem 内部文本左边间距
    int pixelMetric(QStyle::PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;
};
}  // namespace Kiran
