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

#pragma once
#include <QJsonDocument>
#include <QWidget>

namespace Ui
{
class Navigation;
}

class Navigation : public QWidget
{
    Q_OBJECT

public:
    explicit Navigation(QWidget *parent = nullptr);
    ~Navigation();
signals:
    void entryArticlePage(const QString& key);

private:
    Ui::Navigation *ui;
    // 配置文件路径 (://conf/km-config.json)
    QString m_confFilePath;
    // 配置文件解析后的JSON形式
    QJsonDocument m_confJson;

    // 加载导航页配置文件
    bool loadConf();
    // 初始化视图
    void initView();
};
