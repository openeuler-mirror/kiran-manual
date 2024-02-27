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
#include "doc-frame.h"
#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPoint>
#include <QPushButton>
#include <QStyleOption>
#include <QVBoxLayout>
#include "constants.h"

namespace Kiran
{
DocFrame::DocFrame(QWidget* parent, Qt::WindowFlags f)
    : QWidget(parent, f)
{
}

void DocFrame::init()
{
    setAttribute(Qt::WA_Hover, true);
    this->setFixedSize(DOC_FRAME_WIDTH, DOC_FRAME_WIDTH);
    this->setCursor(QCursor(Qt::PointingHandCursor));

    auto innerItemLayout = new QVBoxLayout(this);
    innerItemLayout->setAlignment(Qt::AlignCenter);

    // 背景图片文件路径
    QImage img(m_iconPath);
    int w = img.width();
    // 图片宽高等比例缩放
    int h = img.height();
    // 调整图片中宽高最大者至maxSide
    int maxSide = 96;
    if (w >= h)
    {
        double scale = maxSide / double(w);
        w = maxSide;
        h *= scale;
    }
    else
    {
        double scale = maxSide / double(h);
        h = maxSide;
        w *= scale;
    }

    // 声明图片按钮
    auto iBtn = new QPushButton();
    iBtn->setMaximumWidth(WINDOW_WIDTH / m_maxPerLine);
    QString styleSheet = QString("border-image: url(%1);").arg(m_iconPath);
    iBtn->setStyleSheet(styleSheet);
    iBtn->setFixedSize(w, h);

    connect(iBtn, &QPushButton::clicked, this, [=]()
            {
                auto clickedButton = qobject_cast<QPushButton*>(sender());
                if (clickedButton)
                {
                    emit clicked();
                }
            });
    // 声明条目标题
    auto titleLabel = new QLabel(m_itemName, this);
    titleLabel->setAlignment(Qt::AlignCenter);
    // 添加图片按钮和条目标题
    innerItemLayout->addWidget(iBtn);
    innerItemLayout->addWidget(titleLabel);
}

void DocFrame::mousePressEvent(QMouseEvent* event)
{
    m_mousePos = QPoint(event->x(), event->y());
}

void DocFrame::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_mousePos == QPoint(event->x(), event->y()))
        emit clicked();
}

void DocFrame::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    QStyle::State state;

    opt.initFrom(this);
    state = opt.state;

    // 绘制圆角
    QPainterPath painterPath;
    QRectF qRect = opt.rect;
    qRect.adjust(0.5, 0.5, -0.5, -0.5);
    painterPath.addRoundedRect(qRect, m_radius, m_radius);

    auto getStateFunc = [this](QStyle::State state) -> StylePalette::ColorState
    {
        if (!(state & QStyle::State_Enabled))
        {
            return StylePalette::Disabled;
        }
        else if (state & QStyle::State_Sunken)
        {
            return StylePalette::Active;
        }
        else if ((state & QStyle::State_MouseOver) && testAttribute(Qt::WA_Hover))
        {
            return StylePalette::Hover;
        }
        else
        {
            return StylePalette::Normal;
        }
    };

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    auto kiranPalette = StylePalette::instance();
    if (m_drawBackground)
    {
        QColor backgroundColor;
        backgroundColor = kiranPalette->color(getStateFunc(state),
                                              StylePalette::Widget,
                                              StylePalette::Background);
        painter.fillPath(painterPath, backgroundColor);
    }

    QWidget::paintEvent(event);
}
void DocFrame::setDocFrame(QString& itemName, QString& iconPath, QString& filePath, int maxPerLine)
{
    m_itemName = itemName;
    m_iconPath = iconPath;
    m_filePath = filePath;
    m_maxPerLine = maxPerLine;
    init();
}
}  // namespace Kiran