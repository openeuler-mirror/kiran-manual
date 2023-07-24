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
#include "base-window.h"
#include "document.h"
#include "navigation.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class Window;
}
QT_END_NAMESPACE

class Window : public BaseWindow
{
    Q_OBJECT

public:
    explicit Window(QMainWindow *parent = nullptr);
    ~Window() override;

public slots:
    void documentPageLoader(const QString& mdfPath);
    void navigationPageLoader(const QString& key);
private:
    Ui::Window *m_ui;
    Navigation *m_navigation{};
    Document *m_document{};

    void init();
    void initTitleBar();
};
