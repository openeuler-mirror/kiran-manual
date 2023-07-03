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

#include "navigation.h"
#include "ui_navigation.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

Navigation::Navigation(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Navigation)
{
    ui->setupUi(this);
    // 设定配置文件路径 (://conf/km-config.json)
    this->m_confFilePath = "/home/skyzcyou/Documents/km-config.json";
    // 从配置文件加载配置
    loadConf();
    // 初始化视图
    initView();
}

Navigation::~Navigation()
{
    delete ui;
}

/**
 * @brief Navigation::loadConf 以 JSON 形式加载配置文件
 * @return bool 是否正常解析配置文件为 JSON 数据
 */
bool Navigation::loadConf()
{
    //判断路径以及是否正常打开
    if (m_confFilePath.isEmpty())
    {
        return false;
    }
    QFile file(m_confFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    //读取数据后关闭文件
    const QByteArray rawData = file.readAll();
    file.close();

    //解析为Json文档
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(rawData, &jsonError);
    //是否正常解析Json数据
    if (jsonDoc.isNull() || jsonDoc.isEmpty() || jsonError.error != QJsonParseError::NoError)
    {
        return false;
    }
    m_confJson = jsonDoc;
    return true;
}
/**
 * @brief Navigation::initView 初始化视图
 * @details
 */
void Navigation::initView()
{
    // 定义最外部的垂直布局容器
    QVBoxLayout *homeLayout = new QVBoxLayout(this);

    QJsonObject obj = m_confJson.object();
    QJsonArray kmArray = obj.value("kiran-manual").toArray();
    for (auto it = kmArray.constBegin(); it != kmArray.constEnd(); ++it)
    {
        QJsonObject obj = it->toObject();
        QString itType = obj.keys()[0];
        qDebug() << "Scaned Type: " << itType << endl;

        // 创建分类块
        QVBoxLayout *typeLayout = new QVBoxLayout();
        typeLayout->addWidget(new QLabel(itType));
        QHBoxLayout *itemLayout = new QHBoxLayout();
        typeLayout->addLayout(itemLayout);

        QJsonArray itArray = it->toObject().value(itType).toArray();
        for (auto i = itArray.constBegin(); i != itArray.constEnd(); ++i)
        {
            QJsonObject o = i->toObject();
            QString iName = o.value("name").toString();
            qDebug() << "Scaned Item: " << iName << endl;
            QPushButton *iBtn = new QPushButton(iName);
            iBtn->setMaximumWidth(100);
            connect(iBtn, &QPushButton::clicked, this, [=]() {
                QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
                if (clickedButton)
                {
                    qDebug() << "Button clicked: " << itType + "->" + iName;
                    //用 emit 发信号
                    emit entryArticlePage(iName);
                }
            });
            // 添加条目块
            itemLayout->addWidget(iBtn);
        }

        // 添加分类块
        homeLayout->addLayout(typeLayout);
    }
}
