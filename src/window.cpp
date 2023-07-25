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
#include "constants.h"
#include <search-edit/search-edit.h>
#include <QAction>
#include <QHBoxLayout>

Window::Window(QWidget* parent) : KiranTitlebarWindow(parent)
{
    initTitleBar();
    init();
}

Window::~Window()
= default;

// 槽函数：加载文档页面
void Window::documentPageLoader(const QString& key)
{
    m_document->m_mdFilePath = key;
    m_document->reloadDocument();
    // 将路径入栈，实现前进后退功能

    m_stackedWidget->setCurrentWidget(m_document);
}
// 槽函数：加载导航页面
void Window::navigationPageLoader(const QString& key)
{
    m_stackedWidget->setCurrentWidget(m_navigation);
}
// 初始化导航页视图
void Window::init()
{
    // 声明 Navigation, Document 页面
    m_stackedWidget = new QStackedWidget(this);
    m_navigation = new Navigation(this);
    m_document = new Document(this);
    // 将主页和文章页面添加到 QStackedWidget 中, 并设定主页
    m_stackedWidget->addWidget(m_navigation);
    m_stackedWidget->addWidget(m_document);
    m_stackedWidget->setCurrentWidget(m_navigation);
    m_stackedWidget->setStyleSheet("QStackedWidget { border-radius: 6px;}");

    auto outWidget = new QWidget(this);
    auto outLayout = new QVBoxLayout(outWidget);
    // 页面颜色
    QPalette pal(this->palette());
    pal.setColor(QPalette::Window, Qt::black);
    outWidget->setAutoFillBackground(true);
    outWidget->setPalette(pal);
    outWidget->setStyleSheet("border-radius: 6px;");
    outLayout->addWidget(m_stackedWidget);
    // 初始化中心显示窗口
    setWindowContentWidget(outWidget);
    // 关联页面切换信号到槽函数
    connect(m_navigation, &Navigation::docPageClicked, this, &Window::documentPageLoader);
    connect(m_document, &Document::backHomeClicked, this, &Window::navigationPageLoader);
}
void Window::initTitleBar()
{
    // 初始化标题栏
    setTitleBarHeight(40);
    setButtonHints(KiranTitlebarWindow::TitlebarMinMaxCloseHints);
    setTitlebarColorBlockEnable(true);
    setIcon(QIcon::fromTheme("kiran-control-panel"));
    setTitle(tr(PROJECT_NAME));

    // 添加搜索框
    auto* searchBox = new SearchEdit(this);
    searchBox->setPlaceholderText(tr("Enter keywords to search"));
    searchBox->setFixedWidth(this->width()/2);
    searchBox->setFocusPolicy(Qt::ClickFocus);

    getTitlebarCustomLayout()->addWidget(searchBox);
    setTitlebarCustomLayoutAlignHCenter(true);
}
void Window::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
}
