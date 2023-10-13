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

#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
#include "constants.h"
#include "doc-frame/doc-frame.h"
#include "kiran-log/qt5-log-i.h"
#include "kiran-style/style-palette.h"
#include "string"
#include "ui_navigation.h"

Navigation::Navigation(QWidget *parent)
    : KiranColorBlock(parent),
      m_ui(new Ui::Navigation)
{
    m_ui->setupUi(this);
    init();
}

Navigation::~Navigation()
{
    delete m_ui;
}

void Navigation::init()
{
  // 定义最外部的垂直布局容器
  auto homeWidget = new QWidget(this);
  homeWidget->setContentsMargins(15, 15, 15, 15);
  auto homeLayout = new QVBoxLayout(homeWidget);

  // 载入配置文件
  QSettings settings(m_confFilePath, QSettings::IniFormat);
  settings.setIniCodec("UTF-8");

  // 获取公共信息
  settings.beginGroup(CONFIG_KEY_DOCUMENT);

  QStringList languageSupport = settings.value(CONFIG_KEY_LANGUAGE_SUPPORT).toStringList();
  // 获取当前系统的语言环境
  QLocale locale = QLocale::system();
  QString localName = locale.name();
  // 目前只支持中英文，不在支持语言中则为英文
  if (!languageSupport.contains(localName))
  {
    KLOG_INFO() << "dont support language-" + localName + ", auto change to en_US";
    localName = languageSupport.at(0);
  }

  QString localFlag = "[" + localName + "]";

  // 程序内 categories 使用英文，需要显示时再转为其他语言
  QStringList categories = settings.value(CONFIG_KEY_CATEGORIES_ENUS).toStringList();
  QStringList categoriesLocal = settings.value(CONFIG_KEY_CATEGORIES + localFlag).toStringList();
  settings.endGroup();

  // 输出每个分类下的 FileName
  for (auto it = categories.begin(); it != categories.end(); ++it)
  {
    int index = std::distance(categories.begin(), it);
    // "Category" 分两个，一是原始 categoryRaw 二是根据语言环境翻译后到 categoryLocal
    const QString &categoryRaw = *it;
    const QString &categoryLocal = categoriesLocal.at(index);

    // numberPerRow 用于规定每行数量
    // maxPerLine 用于计算 在 numberPerRow 下每一块的宽度
    int numberPerRow = 4;
    int maxPerLine = 7;

    // 声明分类块
    auto typeWidget = new QWidget();
    auto typeLayout = new QVBoxLayout(typeWidget);

    auto itemWidget = new QWidget();
    auto itemLayout = new QGridLayout(itemWidget);
    itemLayout->setAlignment(Qt::AlignLeft);
    itemLayout->setContentsMargins(50, 15, 0, 0);

    auto categoryLabel = new QLabel(categoryLocal);
    categoryLabel->setMaximumHeight(15);
    typeLayout->addWidget(categoryLabel);
    typeLayout->addWidget(itemWidget);

    // 获取该分类下的 item
    QString itemsKey = CONFIG_KEY_DOCUMENT"/" + categoryRaw + CONFIG_KEY_ITEM;
    QStringList items = settings.value(itemsKey).toStringList();
    int count = 0;
    foreach (const QString &item, items)
    {
      settings.beginGroup(CONFIG_KEY_DOCUMENT" " + categoryRaw + " " + item);
      QString itemName = settings.value(CONFIG_KEY_NAME + localFlag).toString();
      QString fileName = settings.value(CONFIG_KEY_FILENAME).toString();
      QString filePath = MARKDOWNS_FOLDER + localName + "/" + fileName;
      QString iconPath = IMAGE_FOR_NAV_FOLDER + settings.value(CONFIG_KEY_ICON).toString();
      settings.endGroup();
      // 声明条目块
      auto innerItemWidget = new DocFrame();
      innerItemWidget->setDocFrame(itemName,iconPath,filePath,maxPerLine);

      connect(innerItemWidget, &DocFrame::clicked, this, [=]()
      {
                  emit documentBlockClicked(filePath);
      });

      // 添加条目块
      itemLayout->addWidget(innerItemWidget, count / numberPerRow, count % numberPerRow);
      itemLayout->setSpacing(80);
      itemLayout->setVerticalSpacing(40);

      count++;
    }
    // 添加分类块
    homeLayout->addWidget(typeWidget);
  }
  homeLayout->addStretch();
  m_ui->scrollArea->setWidget(homeWidget);
  m_ui->scrollArea->setWidgetResizable(true);
}
