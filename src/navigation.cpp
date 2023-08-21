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
#include <QScrollArea>
#include <QSettings>
#include <QVBoxLayout>
#include "kiran-frame/kiran-frame.h"
#include "kiran-log/qt5-log-i.h"
#include "kiran-style/style-palette.h"
#include "scroll-bar/scroll-bar.h"
#include "string"

Navigation::Navigation(QWidget *parent)
    : KiranColorBlock(parent)
{
    init();
}

Navigation::~Navigation() = default;

void Navigation::init()
{
    auto outLayout = new QVBoxLayout(this);
    outLayout->setMargin(0);
    auto scrollArea = new QScrollArea(this);
    outLayout->addWidget(scrollArea);

    auto navScrollBar = new ScrollBar(this);
    scrollArea->setVerticalScrollBar(navScrollBar);

    // 定义最外层 Widget
    auto homeWidget = new QWidget(this);
    outLayout->addWidget(homeWidget);
    homeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    homeWidget->setContentsMargins(15, 15, 15, 15);
    // 定义最外部的垂直布局容器
    auto homeLayout = new QVBoxLayout(homeWidget);

    // 载入配置文件
    QSettings settings(m_confFilePath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    // 获取公共信息
    settings.beginGroup("Document");

    QStringList languageSupport = settings.value("LanguageSupport").toStringList();
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
    QStringList categories = settings.value("Categories[en_US]").toStringList();
    QStringList categoriesLocal = settings.value("Categories" + localFlag).toStringList();
    settings.endGroup();

    // 输出每个分类下的 FileName
    for (auto it = categories.begin(); it != categories.end(); ++it)
    {
        int index = std::distance(categories.begin(), it);
        // "Category" 分两个，一是原始 categoryRaw 二是根据语言环境翻译后到 categoryLocal
        const QString &categoryRaw = *it;
        const QString &categoryLocal = categoriesLocal.at(index);
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
        QString itemsKey = "Document/" + categoryRaw + "Item";
        QStringList items = settings.value(itemsKey).toStringList();
        int count = 0;
        foreach (const QString &item, items)
        {
            settings.beginGroup("Document " + categoryRaw + " " + item);
            QString itemName = settings.value("Name" + localFlag).toString();
            QString fileName = settings.value("FileName").toString();
            QString filePath = MARKDOWNS_FOLDER + localName + "/" + fileName;
            QString iconPath = IMAGE_FOR_NAV_FOLDER + settings.value("Icon").toString();
            settings.endGroup();
            // 声明条目块
            auto innerItemWidget = new KiranFrame();
            innerItemWidget->setDrawBorder(false);
            auto innerItemLayout = new QVBoxLayout(innerItemWidget);
            innerItemLayout->setAlignment(Qt::AlignCenter);

            // 背景图片文件路径
            QImage img(iconPath);
            int w = img.width();
            // 图片宽高等比例缩放
            int h = img.height();
            // 调整图片中宽高最大者至maxSide
            int maxSide = 100;
            if (w >= h)
            {
                double scale = maxSide / double(w);
                w = maxSide;
                h *= scale;
            }
            else
            {
                double scale = maxSide / double(h);
                h = maxSide;
                w *= scale;
            }

            // 声明图片按钮
            auto iBtn = new QPushButton();
            iBtn->setMaximumWidth(parentWidget()->width() / maxPerLine);
            QString styleSheet = QString("border-image: url(%1);").arg(iconPath);
            iBtn->setStyleSheet(styleSheet);
            iBtn->setFixedSize(w, h);

            // clang-format off
            connect(iBtn, &QPushButton::clicked, this, [=]() {
                auto clickedButton = qobject_cast<QPushButton *>(sender());
                if (clickedButton)
                {
                    emit documentBlockClicked(filePath);
                }
            });
            // clang-format on
            // 声明条目标题
            auto titleLabel = new QLabel(itemName, innerItemWidget);
            titleLabel->setAlignment(Qt::AlignCenter);
            // Fixme: 以下代码用一种不好的方式解决 categoryLabel, titleLabel 文字不跟随主题变化到问题
            // note: 要跟随主题变化要求控件不能设置样式表，如有样式表则会导致主题透传失败
            // 后期优化
            using namespace Kiran;
            auto stylePalette = StylePalette::instance();
            // clang-format off
            connect(stylePalette, &StylePalette::themeChanged, this, [=](Kiran::PaletteType paletteType) {
                QColor qColor = stylePalette->color(StylePalette::Normal,
                                                    StylePalette::Widget,
                                                    StylePalette::Foreground);
                QPalette palette{};
                palette.setColor(QPalette::WindowText, qColor);
                categoryLabel->setPalette(palette);
                titleLabel->setPalette(palette);
            });
            // clang-format on
            // 添加图片按钮和条目标题
            innerItemLayout->addWidget(iBtn);
            innerItemLayout->addWidget(titleLabel);
            innerItemWidget->setFixedSize(150, 150);
            // 添加条目块
            itemLayout->addWidget(innerItemWidget, count / 4, count % numberPerRow);
            itemLayout->setSpacing(80);
            itemLayout->setVerticalSpacing(40);

            count++;
        }
        // 添加分类块
        homeLayout->addWidget(typeWidget);
    }
    homeLayout->addStretch();
    scrollArea->setWidget(homeWidget);
    scrollArea->setWidgetResizable(true);
}
