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

#include "document.h"
#include "ui_document.h"
#include "highlighter.h"

#include <qt5-log-i.h>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include <string>
#include "markdown-parser.h"

Document::Document(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Document)
{
    m_ui->setupUi(this);
    init();

}

Document::~Document()
{
    delete m_ui;
}

void Document::init()
{
    // 组件初始化
    m_ui->textBrowser->setOpenExternalLinks(true);
    // QTextBrowser 最外层组件样式，组件内部的渲染样式需要使用源 HTML 中到CSS来调整
    m_ui->textBrowser->setStyleSheet("\
                                    QTextBrowser { background-color: #ffffff; padding-top: 16px; padding-left:5px;}\
    ");
    m_ui->pushButtonBackHome->setText(tr("返回主页"));
    // 代码高亮
    Highlighter *highlighter= new Highlighter(m_ui->textBrowser->document());
    m_ui->treeWidget->setHeaderHidden(true);
    // 关联到槽函数
    connect(m_ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &Document::tocItemScrollToAnchor);
    connect(m_ui->pushButtonSearch, &QPushButton::clicked, this, &Document::searchKeyword);
    connect(m_ui->pushButtonBackHome, &QPushButton::clicked, this, &Document::backHome);
}

 // 返回解析 Markdown 成功后的 HTML 字符串
QString Document::mdFile2HtmlStr(const QString& mdPath)
{
    using namespace std;
    string mdFilePath = string((const char *)mdPath.toLocal8Bit());
    MarkdownParser markdownParser(mdFilePath);
    markdownParser.transfer();
    string htmlStr = markdownParser.html();

    QJsonObject rootObject = markdownParser.buildJSONTOC();
    renderCatalog(rootObject);

    return QString(QString::fromLocal8Bit(htmlStr.c_str()));
}

void Document::tocItemScrollToAnchor(QTreeWidgetItem *item, int column)
{
    QString itemName = item->text(column);
    m_ui->textBrowser->scrollToAnchor(itemName);
}

// DEBUG 信息，当打开.md文档时，将 markdown2html 模块输出的 html 文档输出到文件，调试使用
void Document::htmlStrSaveToFile(QString& fileName, QString& hStr)
{
    if (fileName.endsWith(".md")) {
        QString pureFileName = fileName.split(".").first();
        QString htmlFilePath = "html/" + pureFileName + ".html";
        QDir().mkpath("html"); // 创建目录
        QFile hFile(htmlFilePath);
        if (!hFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            KLOG_ERROR() << "Open File Failed: " + htmlFilePath;
            return;
        }
        QTextStream tsOut(&hFile);
        tsOut << hStr;
        KLOG_INFO() << htmlFilePath + " => HTML document save success" << endl;
    }
}

void Document::showTOC(QTreeWidgetItem *root, const QJsonObject& obj, int level)
{
    QTreeWidgetItem *childRoot;
    // 输出当前对象的 heading 属性
    if (obj.contains("heading")) {
        QJsonValue headingValue = obj.value("heading");
        if (headingValue.isString()) {
            QString heading = headingValue.toString();
            childRoot = new QTreeWidgetItem(QStringList() << heading);
            if (root == nullptr)
            {
                m_ui->treeWidget->addTopLevelItem(childRoot);
            }
            else
            {
                root->addChild(childRoot);
            }
        }
    }
    // 递归遍历当前对象的 child 数组
    if (obj.contains("child")) {
        QJsonValue childValue = obj.value("child");
        if (childValue.isArray()) {
            QJsonArray childArray = childValue.toArray();
            for (auto child : childArray) {
                if (child.isObject()) {
                    showTOC(childRoot, child.toObject(), level + 1);
                }
            }
        }
    }
}
// 重新加载文档
void Document::reloadDocument()
{
    // 解析并渲染目标文档
    if (m_mdFilePath.isEmpty()){
        KLOG_ERROR() << "m_mdFilePath is empty!! " << m_mdFilePath;
        return;
    }

    // 原生渲染无法配合当前到样式定制，当前只使用自解析渲染
    // 不同 QT 版本调用不同 Markdown 渲染方法:
    // QT_VERSION >= 5.14 QT 原生渲染函数
    // QT_VERSION < 5.14     自解析渲染函数
#if QT_VERSION >= QT_VERSION_CHECK(6, 16, 0)
    m_ui->textBrowser->setSource(m_mdFilePath);
#else
    QString hStr = mdFile2HtmlStr(m_mdFilePath);
    // DELETE ME . DEBUG 需要，保存解析后的 html 到文件
    //QString testFileName = "testFileName.md";
    //htmlStrSaveToFile(testFileName,hStr);
    m_ui->textBrowser->setHtml(hStr);
#endif
}

void Document::searchKeyword()
{
    QString keyword = m_ui->lineEditKeyword->text();

    if (keyword.trimmed().isEmpty()) {
        QMessageBox::information(this, tr("关键字为空"), tr("The search field is empty."));
    } else {
        QTextDocument *document = m_ui->textBrowser->document();
        QTextCursor cursor(document);

        // 搜索文本
        int count = 0;
        QTextCharFormat format;
        format.setBackground(Qt::yellow);

        // 从上一次匹配项的位置开始搜索
        if (!m_lastMatch.isNull()) {
            cursor.setPosition(m_lastMatch.position() + m_lastMatch.selectedText().length());
        }
        while (!cursor.isNull() && !cursor.atEnd()) {
            cursor = document->find(keyword, cursor, QTextDocument::FindWholeWords);
            if (!cursor.isNull()) {
                cursor.mergeCharFormat(format);
                ++count;
                // 将光标定位到匹配项的位置
                m_ui->textBrowser->setTextCursor(cursor);
                m_ui->textBrowser->ensureCursorVisible();
                // 记录匹配项的位置
                m_lastMatch = cursor;
                // 只找下一项
                break;
            }
        }
        // 显示搜索结果
        if (count > 0) {
            // QMessageBox::information(this, tr("Search results"), tr("Found %1 occurrences of '%2'.").arg(count).arg(keyword));
        } else {
            QMessageBox::information(this, tr("Search results"), tr("No more occurrences of '%1' found.").arg(keyword));
            m_lastMatch = QTextCursor();
            clearSearchHighlights();
        }
    }
}
void Document::clearSearchHighlights()
{
    QTextDocument *document = m_ui->textBrowser->document();
    QTextCursor cursor(document);

    // 清除上次高亮的文本
    QTextCharFormat format;
    format.clearBackground();

    while (!cursor.isNull() && !cursor.atEnd()) {
        cursor = document->find(m_ui->lineEditKeyword->text(), cursor, QTextDocument::FindWholeWords);
        if (!cursor.isNull()) {
            cursor.mergeCharFormat(format);
        }
    }
}
void Document::backHome()
{
    emit backHomeClicked("HOME");
}
void Document::renderCatalog(QJsonObject& jsonObject)
{
    // 获取目录到 JSON 格式
    m_ui->treeWidget->clear();

    QJsonArray jsonArray = jsonObject["_child"].toArray();
    for (auto obj : jsonArray) {
        if (obj.isObject()) {
            showTOC(nullptr, obj.toObject());
        }
    }
    m_ui->treeWidget->expandAll();
}
