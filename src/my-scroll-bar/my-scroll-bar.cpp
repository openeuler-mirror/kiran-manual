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

#include "my-scroll-bar.h"
#include <QPainter>
MyScrollBar::MyScrollBar(QWidget *parent)
    : QScrollBar(parent)
{
}

MyScrollBar::~MyScrollBar()
= default;

QSize MyScrollBar::sizeHint() const
{
    return QScrollBar::sizeHint();
}

void MyScrollBar::onSetRange(int min, int max)
{
    if (max > 0)
    {
        setVisible(true);
        setGeometry(m_area->rect().width() - width() - 2, 2, width(), height());
    }
    setRange(min, max);
}
void MyScrollBar::paintEvent(QPaintEvent *ev)
{
    //绘制滚动条背景
    QRect rc = this->rect();
    QPainter painter(this);
    QPainterPath path;
    // 设置圆角大小为 6
    path.addRoundedRect(rc, 6, 6);
    // TODO: 颜色跟随主题
    painter.fillPath(path, QColor(Qt::transparent));

    QStyleOptionSlider opt;
    initStyleOption(&opt);

    QRect sliderRc = style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarSlider,this);  //获取滑块控件

    // 绘制滚动条
    // TODO: 颜色跟随主题
    QPainterPath p;
    p.addRoundedRect(sliderRc, 3, 3);
    painter.fillPath(p, QColor(226,226,226,100));
}
void MyScrollBar::setArea(QAbstractScrollArea *area)
{
    area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto bar = area->verticalScrollBar();
    QObject::connect(
        bar, SIGNAL(rangeChanged(int,int)), this, SLOT(onSetRange(int,int)));
    QObject::connect(
        bar, SIGNAL(valueChanged(int)), this, SLOT(setValue(int))
    );
    QObject::connect(
        this, SIGNAL(valueChanged(int)), bar, SLOT(setValue(int))
    );
    setVisible(false);
    m_area = area;

    //设置成自定义的样式
    m_style = new MyScrollStyle();
    setStyle(m_style);
}
