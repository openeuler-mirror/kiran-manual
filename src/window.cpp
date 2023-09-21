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
#include <QAction>
#include <QBitmap>
#include <QPainter>
#include "constants.h"

#include "ui_window.h"
#include "navigation.h"
#include "document.h"

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
    m_ui->document->renderDocument(mdfPath);
    m_ui->stackedWidget->setCurrentWidget(m_ui->document);
}

void Window::switchToNavigation(const QString& key)
{
    m_ui->stackedWidget->setCurrentWidget(m_ui->navigation);
}

void Window::init()
{
    this->setTitleBar();
    setWindowContentWidget(m_ui->palette);
    connect(m_ui->navigation, &Navigation::documentBlockClicked, this, &Window::switchToDocument);
    connect(m_ui->document, &Document::backHomeClicked, this, &Window::switchToNavigation);
}

void Window::setTitleBar()
{
    setTitleBarHeight(TITLE_BAR_HEIGHT);
    setButtonHints(KiranTitlebarWindow::TitlebarMinMaxCloseHints);
    setTitlebarColorBlockEnable(true);
    QPixmap pixmap(TITLE_BAR_ICON_PATH);
    setIcon(pixmap);
    setTitle(tr("kiran manual"));
}
