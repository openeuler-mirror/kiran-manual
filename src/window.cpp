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
#include <kiran-style/style-palette.h>
#include <search-edit/search-edit.h>
#include <QAction>
#include <QStackedWidget>
#include <QBitmap>
#include <QHBoxLayout>
#include <QPainter>
#include "constants.h"
#include "document.h"
#include "navigation.h"
#include "ui_window.h"

Window::Window(QWidget* parent)
    : KiranTitlebarWindow(parent),
      m_ui(new Ui::Window)
{
    m_ui->setupUi(this);
    init();
}

Window::~Window(){
    delete m_ui;
}

void Window::switchToDocument(const QString& mdfPath)
{
    m_documentPage->renderDocument(mdfPath);
    m_pageStacked->setCurrentWidget(m_documentPage);
    // 改变搜索框到搜索域到文档页面
//    m_searchBox->setSearchField(m_documentPage->objectName());
}

void Window::switchToNavigation(const QString& key)
{
    m_pageStacked->setCurrentWidget(m_navigationPage);
    // 改变搜索框到搜索域到导航页面
//    m_searchBox->setSearchField(m_navigationPage->objectName());
}

void Window::init()
{
    setTitleBar();
    // 声明 Navigation, Document 页面
    m_pageStacked = new QStackedWidget(this);
    m_navigationPage = new Navigation(this);
    m_documentPage = new Document(this);
    // 设置 objectName 用于判断搜索域
    m_navigationPage->setObjectName(NAVIGATION_OBJECT_NAME);
    m_documentPage->setObjectName(DOCUMENT_OBJECT_NAME);
    // 添加导航页、文档页到 pageStacked, 并设定主页
    m_pageStacked->addWidget(m_navigationPage);
    m_pageStacked->addWidget(m_documentPage);
    m_pageStacked->setCurrentWidget(m_navigationPage);

    auto outWidget = new QWidget(this);
    auto outLayout = new QVBoxLayout(outWidget);
    outLayout->setMargin(4);
    QPalette pal(this->palette());
    outWidget->setAutoFillBackground(true);
    outWidget->setPalette(pal);
    outWidget->setStyleSheet("border-radius: 6px;");
    outLayout->addWidget(m_pageStacked);

    setWindowContentWidget(outWidget);
    // 关联页面切换信号到槽函数
    connect(m_navigationPage, &Navigation::documentBlockClicked, this, &Window::switchToDocument);
    connect(m_documentPage, &Document::backHomeClicked, this, &Window::switchToNavigation);
    // 关联搜索框搜索信号到槽函数
//    connect(m_searchBox, &SearchEdit::requestSearchTextBrowserNext, m_documentPage, &Document::searchNextKeyword);
//    connect(m_searchBox, &SearchEdit::requestSearchTextBrowserPrev, m_documentPage, &Document::searchPrevKeyword);
//    connect(m_searchBox, &SearchEdit::requestSearchTextBrowserClosed, m_documentPage, &Document::searchKeywordClose);
//    connect(m_searchBox, &SearchEdit::requestSearchKeywordChanged, m_documentPage, &Document::searchKeywordChange);
//    connect(m_documentPage, &Document::keywordCountDone, m_searchBox, &SearchEdit::updateSearchCount);
}

void Window::setTitleBar()
{
    setTitleBarHeight(TITLE_BAR_HEIGHT);
    setButtonHints(KiranTitlebarWindow::TitlebarMinMaxCloseHints);
    setTitlebarColorBlockEnable(true);
    //    setIcon(QIcon::fromTheme("kiran-control-panel"));
    QPixmap pixmap(ICON_PATH);
    setIcon(pixmap);
    setTitle(tr("kiran manual"));

//    m_searchBox = new SearchEdit(this);
//    // 添加搜索框
//    m_searchBox->setPlaceholderText(tr("Enter keywords to search"));
//    m_searchBox->setFixedWidth(this->width() / 2);
//    m_searchBox->setFocusPolicy(Qt::ClickFocus);
//    getTitlebarCustomLayout()->addWidget(m_searchBox);
//    setTitlebarCustomLayoutAlignHCenter(true);
}

