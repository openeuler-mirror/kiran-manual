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

#include <kiranwidgets-qt5/kiran-titlebar-window.h>
#include <QWidget>

class SearchEdit;
class QStackedWidget;
class Navigation;
class Document;

class Window : public KiranTitlebarWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget* parent = nullptr);
    ~Window() override;

public slots:
    void switchToDocument(const QString& mdfPath);
    void switchToNavigation(const QString& key);

private:
    void init();
    void setTitleBar();

    SearchEdit* m_searchBox{};
    QStackedWidget* m_pageStacked{};
    Navigation* m_navigationPage{};
    Document* m_documentPage{};
};
