/**
 * Copyright (c) 2020 ~ 2021 KylinSec Co., Ltd.
 * kiran-session-manager is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 *
 * Author:     youzhengcai <youzhengcai@kylinse.com.cn>
 */

#include "window.h"
#include "ui_window.h"

#include <QDebug>

Window::Window(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::Window)
{
    ui->setupUi(this);
    initView();
}

Window::~Window()
{
    delete ui;
}
/**
 * @brief Window::switchArticlePage
 * @param key: 文件名
 */
void Window::switchArticlePage(const QString& key)
{
    // 根据 key 组装完整的 md 文件路径 	fullPath
    QString fullPath = "/home/skyzcyou/Documents/kiran_manual/" + key + ".md";
    article->m_mdFilePath = fullPath;
    article->reloadArticle();
    // TODO: 将路径入栈，实现前进后退功能

    ui->stackedWidget->setCurrentIndex(3);
}
/**
 * @brief Window::switchHomePage
 * @param key
 */
void Window::switchHomePage(const QString& key)
{
    ui->stackedWidget->setCurrentIndex(2);
}

/**
 * @brief Window::initView 初始化导航页视图
 */
void Window::initView()
{
    // 声明 Navigation, Article 页面
    navigation = new Navigation();
    article = new Article();
    // 将主页和文章页面添加到 QStackedWidget 中, 并设定主页
    ui->stackedWidget->addWidget(navigation);
    ui->stackedWidget->addWidget(article);
    ui->stackedWidget->setCurrentWidget(navigation);
    // 关联页面切换信号到槽函数
    connect(navigation, &Navigation::entryArticlePage, this, &Window::switchArticlePage);
    connect(article, &Article::backHome, this, &Window::switchHomePage);
}
