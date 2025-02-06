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

#include <QWidget>

namespace Kiran
{
class DocFrame : public QWidget
{
    Q_OBJECT
public:
    explicit DocFrame(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    void setDocFrame(QString& itemName, QString& iconPath, QString& filePath, int maxPerLine);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

signals:
    void clicked();

private:
    void init();

private:
    int m_radius = 6;
    QPoint m_mousePos;
    bool m_drawBackground = true;

    QString m_itemName;
    QString m_iconPath;
    QString m_filePath;
    int m_maxPerLine{};
};
}  // namespace Kiran