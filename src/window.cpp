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

#include "window.h"
#include "ui_window.h"
#include "constants.h"

Window::Window(QMainWindow* parent)
    : BaseWindow(parent), m_ui(new Ui::Window)
{
    m_ui->setupUi(this);
    initTitleBar();
    init();
}

Window::~Window()
{
    delete m_ui;
}

// 槽函数：加载文档页面
void Window::documentPageLoader(const QString& key)
{
    m_document->m_mdFilePath = key;
    m_document->reloadDocument();
    // 将路径入栈，实现前进后退功能

    m_ui->stackedWidget->setCurrentIndex(3);
}
// 槽函数：加载导航页面
void Window::navigationPageLoader(const QString& key)
{
    m_ui->stackedWidget->setCurrentIndex(2);
}

// 初始化导航页视图
void Window::init()
{
    // 页面颜色
    QPalette pal(this->palette());
    // #2d2d2d
    pal.setColor(QPalette::Window, QColor(45,45,45));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    this->setWindowTitle(tr(PROJECT_NAME));
    // 声明 Navigation, Document 页面
    m_navigation = new Navigation(this);
    m_document = new Document(this);
    // 将主页和文章页面添加到 QStackedWidget 中, 并设定主页
    m_ui->stackedWidget->addWidget(m_navigation);
    m_ui->stackedWidget->addWidget(m_document);
    m_ui->stackedWidget->setCurrentWidget(m_navigation);
    m_ui->stackedWidget->setStyleSheet("QStackedWidget { background-color: #2d2d2d}");
    // 关联页面切换信号到槽函数
    connect(m_navigation, &Navigation::docPageClicked, this, &Window::documentPageLoader);
    connect(m_document, &Document::backHomeClicked, this, &Window::navigationPageLoader);
}
void Window::initTitleBar()
{
    m_titleBar->setTitleIcon(":/resources/title-icon.png");
    m_titleBar->setBackgroundColor(45, 45, 45);
    m_titleBar->setTitleContent(tr(PROJECT_NAME));
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}
