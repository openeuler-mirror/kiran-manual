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
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QLineEdit>
#include <QHBoxLayout>

#define BUTTON_HEIGHT 15		// 按钮高度;
#define BUTTON_WIDTH 15			// 按钮宽度;
#define EDIT_WIDTH 25			// 按钮宽度;
#define TITLE_HEIGHT 40			// 标题栏高度;

enum ButtonType
{
    MIN_BUTTON = 0,         // 最小化和关闭按钮;
    MIN_MAX_BUTTON ,        // 最小化、最大化和关闭按钮;
    ONLY_CLOSE_BUTTON       // 只有关闭按钮;
};

class MyTitleBar : public QWidget
{
Q_OBJECT

public:
    explicit MyTitleBar(QWidget *parent);
    //这里parent没有给默认值NULL，保证在创建MyTitleBar对象时父指针必须得赋值;且赋值不为NULL;
    ~MyTitleBar() override;

    // 设置标题栏背景色及是否设置标题栏背景色透明;
    void setBackgroundColor(int r, int g, int b , bool isTransparent = false);
    // 设置标题栏图标;
    void setTitleIcon(const QString& filePath , QSize IconSize = QSize(17 , 17));
    // 设置标题内容;
    void setTitleContent(const QString& titleContent , int titleFontSize = 9);
    // 设置标题栏长度;
    void setTitleWidth(int width);
    // 设置标题栏上按钮类型;
    void setButtonType(ButtonType buttonType);
    // 设置标题栏中的标题是否会滚动;具体可以看效果;
    void setTitleRoll();
    // 设置窗口边框宽度;
    void setWindowBorderWidth(int borderWidth);

    // 保存/获取 最大化前窗口的位置及大小;
    void saveRestoreInfo(QPoint point, QSize size);
    void getRestoreInfo(QPoint& point, QSize& size);

private:
    void paintEvent(QPaintEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    // 初始化控件;
    void initControl();
    // 信号槽的绑定;
    void initConnections();
    // 加载样式文件;
    void loadStyleSheet(const QString &sheetName);

signals:
    // 按钮触发的信号;
    void signalButtonMinClicked();
    void signalButtonRestoreClicked();
    void signalButtonMaxClicked();
    void signalButtonCloseClicked();

private slots:
    // 按钮触发的槽;
    void onButtonMinClicked();
    void onButtonRestoreClicked();
    void onButtonMaxClicked();
    void onButtonCloseClicked();

private:
    QLabel* m_pIcon{};                    // 标题栏图标;
    QLabel* m_pTitleContent{};            // 标题栏内容;

    QLineEdit *m_pSearchEdit{}; // 搜索框

    QPushButton* m_pButtonMin{};          // 最小化按钮;
    QPushButton* m_pButtonRestore{};      // 最大化还原按钮;
    QPushButton* m_pButtonMax{};          // 最大化按钮;
    QPushButton* m_pButtonClose{};        // 关闭按钮;

    // 标题栏背景色;
    int m_colorR;
    int m_colorG;
    int m_colorB;

    // 最大化，最小化变量;
    QPoint m_restorePos;
    QSize m_restoreSize;
    // 移动窗口的变量;
    bool m_isPressed;
    QPoint m_startMovePos;
    // 标题栏内容;
    QString m_titleContent;
    // 按钮类型;
    ButtonType m_buttonType;
    // 窗口边框宽度;
    int m_windowBorderWidth;
    // 标题栏是否透明;
    bool m_isTransparent;
};

