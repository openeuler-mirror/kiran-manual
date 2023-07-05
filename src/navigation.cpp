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

#include "navigation.h"
#include "ui_navigation.h"
#include "constants.h"

#include "kiran-log/qt5-log-i.h"
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QStandardPaths>
#include <QVBoxLayout>

Navigation::Navigation(QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui::Navigation)
{
    m_ui->setupUi(this);
    // 设定配置文件路径 (:/conf/km-config.ini)
    this->m_confFilePath = ":/data/km-config.ini";
    // 初始化视图
    init();
}

Navigation::~Navigation()
{
    delete m_ui;
}
void Navigation::init()
{
    // 定义最外部的垂直布局容器
    QVBoxLayout *homeLayout = new QVBoxLayout(this);

    // 载入配置文件
    QString confIniPath = m_confFilePath;
    QSettings settings(confIniPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    // 获取公共信息
    settings.beginGroup("Document");
    QString docDir = settings.value("DocDir").toString();

    QStringList languageSupport = settings.value("LanguageSupport").toStringList();
    // 获取当前系统的语言环境
    QLocale locale = QLocale::system();
    QString localName = locale.name();
    // 目前只支持中英文，不在支持语言中则为英文
    if (!languageSupport.contains(localName))
    {
        localName = languageSupport.at(0);
    }
    QString localFlag = "[" + localName + "]";

    // 程序内 categories 使用英文，需要显示时再转为其他语言
    QStringList categories = settings.value("Categories[en_US]").toStringList();
    QStringList categoriesLocal = settings.value("Categories"+localFlag).toStringList();
    settings.endGroup();

    // 输出每个分类下的FileName
    for (auto it = categories.begin(); it != categories.end(); ++it)
    {
        qint8 index = std::distance(categories.begin(),it);
        // "Category" 分两个，一是原始 categoryRaw 二是根据语言环境翻译后到 categoryLocal
        const QString& categoryRaw = *it;
        const QString& categoryLocal = categoriesLocal.at(index);

        // 创建分类块
        QVBoxLayout *typeLayout = new QVBoxLayout();
        typeLayout->addWidget(new QLabel(categoryLocal));
        QHBoxLayout *itemLayout = new QHBoxLayout();
        typeLayout->addLayout(itemLayout);

        // 获取该分类下的 item
        QString itemsKey = "Document/" + categoryRaw + "Item";
        QStringList items = settings.value(itemsKey).toStringList();
        foreach (const QString& item, items)
        {
            settings.beginGroup("Document "+categoryRaw+" "+item);
            QString itemName = settings.value("Name"+localFlag).toString();
            QString fileName = settings.value("FileName").toString();
            QString filePath = docDir + localName + "/" + fileName;
            QString iconPath = docDir + "icon/" + settings.value("Icon").toString();
            settings.endGroup();
            // 声明条目块
            QPushButton *iBtn = new QPushButton(itemName);
            iBtn->setMaximumWidth(100);
            connect(iBtn, &QPushButton::clicked, this, [=]() {
                QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
                if (clickedButton)
                {
                    //用 emit 发信号 发送文档路径
                    emit docPageClicked(filePath);
                }
            });
            // 添加条目块
            itemLayout->addWidget(iBtn);
        }
        // 添加分类块
        homeLayout->addLayout(typeLayout);
    }
}
