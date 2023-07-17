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

#include <QFile>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
#include "kiran-log/qt5-log-i.h"
#include "style-palette.h"

Navigation::Navigation(QWidget *parent)
    : QWidget(parent),
      m_ui(new Ui::Navigation)
{
    m_ui->setupUi(this);
    // 设定配置文件路径 (:/conf/km-config.ini)
    this->m_confFilePath = CONF_FILE_PATH;
    // 初始化视图
    init();
}

Navigation::~Navigation()
{
    delete m_ui;
}
// 初始化视图
void Navigation::init()
{
    // 定义最外层 Widget
    auto *homeWidget = new QWidget(this);
    // 定义最外部的垂直布局容器
    auto *homeLayout = new QVBoxLayout(homeWidget);

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
        KLOG_INFO() << "dont support language-" + localName + ", auto change to en_US";
        localName = languageSupport.at(0);
    }
    QString localFlag = "[" + localName + "]";

    // 程序内 categories 使用英文，需要显示时再转为其他语言
    QStringList categories = settings.value("Categories[en_US]").toStringList();
    QStringList categoriesLocal = settings.value("Categories"+localFlag).toStringList();
    settings.endGroup();

    // 输出每个分类下的 FileName
    for (auto it = categories.begin(); it != categories.end(); ++it)
    {
        qint8 index = std::distance(categories.begin(),it);
        // "Category" 分两个，一是原始 categoryRaw 二是根据语言环境翻译后到 categoryLocal
        const QString& categoryRaw = *it;
        const QString& categoryLocal = categoriesLocal.at(index);

        // 声明分类块
        // 此处不会造成内存泄漏，因为两个对象会绑定到父布局
        QVBoxLayout *typeLayout = new QVBoxLayout();
        int maxPerLine = 7;

        QHBoxLayout *itemLayout = new QHBoxLayout();
        itemLayout->setAlignment(Qt::AlignLeft);
        itemLayout->setMargin(10);

        typeLayout->addWidget(new QLabel(categoryLocal));
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
            QWidget *innerItemWidget = new QWidget();
            QVBoxLayout *innerItemLayout = new QVBoxLayout(innerItemWidget);
            innerItemLayout->setAlignment(Qt::AlignCenter);

            // ==========
            QString imgPathStr = ":/images/Qt.png"; // 背景图片文件路径
            QString styleSheet = QString("QPushButton{border-image: url(%1);}").arg(imgPathStr);
            QPushButton *btn = new QPushButton(this);
            btn->setStyleSheet(styleSheet);
            QImage img(imgPathStr);
            int w = img.width();
            int h = img.height();  // 图片宽高等比例缩放
            int maxSide = 100;     // 调整图片中宽高最大者至maxSide
            if(w >= h){
                double scale = maxSide / double(w);
                w = maxSide;
                h *= scale;
            }else{
                double scale = maxSide / double(h);
                h = maxSide;
                w *= scale;
            }
            btn->setFixedSize(w,h);
            //=============

            QPushButton *iBtn = new QPushButton(innerItemWidget);
            iBtn->setMaximumWidth(parentWidget()->width() / maxPerLine);
            iBtn->setIcon(QIcon("/usr/local/share/kiran-manual/data/manual-books/images/nav/clion.png"));
            iBtn->setIconSize(QSize(parentWidget()->width() / maxPerLine, parentWidget()->width() / maxPerLine));
            iBtn->setStyleSheet("background-color: #393939;");
            connect(iBtn, &QPushButton::clicked, this, [=]() {
                QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
                if (clickedButton)
                {
                    emit docPageClicked(filePath);
                }
            });
            innerItemLayout->addWidget(iBtn);
            innerItemLayout->addWidget(new QLabel(itemName));

            // 样式调整
            QPalette pal(innerItemWidget->palette());
            pal.setColor(QPalette::Background, QColor(57,57,57)); // #2d2d2d
            innerItemWidget->setAutoFillBackground(true);
            innerItemWidget->setPalette(pal);

            // 添加条目块
            itemLayout->addWidget(innerItemWidget);
        }
        // 添加分类块
        homeLayout->addLayout(typeLayout);
    }
    this->LoadStyleSheet(":/data/styles/navigation.qss");
}
// 加载 QSS 样式文件
bool Navigation::LoadStyleSheet(const QString &StyleSheetFile)
{
    QFile file(StyleSheetFile);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::information(this,"Tip", file.errorString());
        return false;
    }
    this->setStyleSheet(file.readAll());
    file.close();
    return true;
}
