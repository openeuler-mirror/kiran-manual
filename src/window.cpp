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
#include <kiran-message-box.h>
#include <kiran-style/style-global-define.h>
#include <kiran-style/style-palette.h>
#include <search-edit/search-edit.h>
#include <QAction>
#include <QBitmap>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include "constants.h"

Window::Window(QWidget* parent) : KiranTitlebarWindow(parent)
{
    initTitleBar();
    init();

    // 关联搜索框搜索信号
    connect(m_searchBox, &SearchEdit::requestSearchTextBrowserNext, m_document, &Document::searchNextKeyword);
    connect(m_searchBox, &SearchEdit::requestSearchTextBrowserPrev, m_document, &Document::searchPrevKeyword);
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
    // 改变搜索框到搜索域到文档页面
    m_searchBox->setSearchFiled(m_document->objectName());
}
// 槽函数：加载导航页面
void Window::navigationPageLoader(const QString& key)
{
    m_stackedWidget->setCurrentWidget(m_navigation);
    // 改变搜索框到搜索域到导航页面
    m_searchBox->setSearchFiled(m_navigation->objectName());
}
// 初始化导航页视图
void Window::init()
{
    // 声明 Navigation, Document 页面
    m_stackedWidget = new QStackedWidget(this);
    m_navigation = new Navigation(this);
    m_navigation->setObjectName("Navigation");
    m_document = new Document(this);
    // 将主页和文章页面添加到 QStackedWidget 中, 并设定主页
    m_stackedWidget->addWidget(m_navigation);
    m_stackedWidget->addWidget(m_document);
    m_stackedWidget->setCurrentWidget(m_navigation);
//    m_stackedWidget->setStyleSheet("QStackedWidget { border-radius: 6px;}");

    auto outWidget = new QWidget(this);
    auto outLayout = new QVBoxLayout(outWidget);
    // 缝隙宽度
    outLayout->setMargin(4);
    // 页面颜色
    QPalette pal(this->palette());
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
//    setIcon(QIcon::fromTheme("kiran-control-panel"));
    QPixmap pixmap(ICON_PATH);
    setIcon(pixmap);
    setTitle(tr(PROJECT_NAME));

    m_searchBox = new SearchEdit(this);
    // 添加搜索框
    m_searchBox->setPlaceholderText(tr("Enter keywords to search"));
    m_searchBox->setFixedWidth(this->width()/2);
    m_searchBox->setFocusPolicy(Qt::ClickFocus);


    getTitlebarCustomLayout()->addWidget(m_searchBox);
    setTitlebarCustomLayoutAlignHCenter(true);
}
void Window::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
}
