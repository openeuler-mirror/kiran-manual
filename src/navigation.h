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
#include <kiranwidgets-qt5/kiran-color-block.h>
#include <QWidget>
#include "constants.h"

namespace Ui
{
class Navigation;
}

class Navigation : public KiranColorBlock
{
    Q_OBJECT

public:
    explicit Navigation(QWidget* parent = nullptr);
    ~Navigation() override;

signals:
    void documentBlockClicked(const QString& key);

private:
    void init();

    Ui::Navigation* m_ui;
    QString m_confFilePath = CONF_FILE_PATH;
};
