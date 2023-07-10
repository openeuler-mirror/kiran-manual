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

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include <QtDebug>
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
    m_ui->pushButtonBackward->setEnabled(false);
    m_ui->pushButtonForward->setEnabled(false);
    // 代码高亮
    Highlighter *highlighter= new Highlighter(m_ui->textBrowser->document());
    
    // 关联到槽函数
    connect(m_ui->pushButtonBackward, &QPushButton::clicked, m_ui->textBrowser,&QTextBrowser::backward);
    connect(m_ui->pushButtonForward, &QPushButton::clicked, m_ui->textBrowser, &QTextBrowser::forward);
    connect(m_ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &Document::onTreeWidgetItemDoubleClicked);
    connect(m_ui->pushButtonSearch, &QPushButton::clicked, this, &Document::searchKeyword);
    connect(m_ui->pushButtonBackHome, &QPushButton::clicked, this, &Document::onPushButtonBackHomeClicked);
    connect(m_ui->textBrowser, &QTextBrowser::forwardAvailable, this, &Document::onTextBrowserForwardAvailable);
    connect(m_ui->textBrowser, &QTextBrowser::backwardAvailable, this, &Document::onTextBrowserBackwardAvailable);

    QString dirPath = "/home/skyzcyou/Documents/manual_book/";
    QTreeWidgetItem *root = new QTreeWidgetItem(m_ui->treeWidget);
    root->setText(0, "manual_book");
    QJsonObject rootJsonObj;
    showDirTree(root, dirPath, rootJsonObj);

}
/**
 * @brief Document::mdFile2HtmlStr
 * @param mdPath: Markdown 文档路径
 * @return QString htmlStr: 返回解析 Markdown 成功后的 HTML 字符串
 */
QString Document::mdFile2HtmlStr(const QString& mdPath)
{
    using namespace std;
    string mp = string((const char *)mdPath.toLocal8Bit());
    MarkdownParser mk(mp);
    mk.transfer();
    string mkStr = mk.html();
    return QString(QString::fromLocal8Bit(mkStr.c_str()));
}

void Document::onTreeWidgetItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString fileName = item->text(column);

    QStringList filePath;
    while (item != nullptr)
    {   //获取 itemFile 名称
        filePath << item->text(0);
        //将itemFile指向父item
        item = item->parent();
    }
    QString strpath;
    //cout<<filepath.size()<<endl;
    //QStringList 类 filepath 反向存着初始item的路径
    for (int i = (filePath.size() - 1); i >= 0; i--)      {
        //将filepath反向输出，相应的加入’/‘
        strpath += filePath.at(i);
        if (i != 0)
            strpath += "/";
    }
    qDebug() << "Full Name: " << strpath << endl;

    // TODO: Change this Path
    QString hStr = mdFile2HtmlStr("/home/skyzcyou/Documents/" + strpath);

    // DEBUG: Save Html to File
    // TODO: DEBUG 信息，当打开.md文档时，将 markdown2html 模块输出的 html 文档输出到文件，调试使用
    if (fileName.endsWith(".md"))
    {
        QFile hFile(fileName + ".html");
        if (!hFile.open(QIODevice::WriteOnly))
        {
            return;
        }
        QTextStream tsOut(&hFile);
        tsOut << hStr;
        qDebug() << fileName + "=> HTML Document Save Success";
    }
    qDebug() << "前进? " << m_ui->textBrowser->isForwardAvailable();
    qDebug() << "后退? " << m_ui->textBrowser->isBackwardAvailable();

    m_ui->textBrowser->setHtml(hStr);
}

// FIXME: 获取文档目录
QFileInfoList Document::showDirTree(QTreeWidgetItem *root, const QString &path, QJsonObject &parentJsonObj)
{
    QDir dir(path);
    QDir dir_file(path);
    // Traversal folder add into Widget
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (int i = 0; i != folder_list.size(); i++)
    {
        // JSON of current node
        QJsonObject subdirJsonObj;
        subdirJsonObj["type"] = QJsonValue("dir");

        // get absolute path
        QString namepath = folder_list.at(i).absoluteFilePath();
        const QFileInfo&folderInfo = folder_list.at(i);

        // get folder name
        QString name = folderInfo.fileName();

        // skip file name star with "_"
        if (name.startsWith("_"))
        {
            continue;
        }
        QTreeWidgetItem *childRoot = new QTreeWidgetItem(QStringList() << name);
        root->addChild(childRoot);
        childRoot->setIcon(0, QIcon(path + "_images/_self/folder.png"));
        childRoot->setText(0, name);

        root->addChild(childRoot);
        QFileInfoList child_file_list = showDirTree(childRoot, namepath, subdirJsonObj);
        file_list.append(child_file_list);
        file_list.append(name);
        // alias
        subdirJsonObj["alias"] = QJsonValue(name);
        parentJsonObj[name] = subdirJsonObj;
    }

    // add file in path
    dir_file.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir_file.setSorting(QDir::Name);
    QFileInfoList list_file = dir_file.entryInfoList();

    QJsonArray jsonArray;
    for (const auto& fileInfo : list_file)
    {
        QString name2 = fileInfo.fileName();
        QTreeWidgetItem *child = new QTreeWidgetItem(QStringList() << name2);
        child->setIcon(0, QIcon(path + "_images/_self/markdown.png"));
        child->setText(0, name2);
        root->addChild(child);

        QJsonObject fileJsonObj;
        fileJsonObj["name"] = QJsonValue(name2);
        // alias: TODO:set document head?
        fileJsonObj["alias"] = QJsonValue(name2);
        fileJsonObj["type"] = QJsonValue("file");
        fileJsonObj["size"] = QJsonValue(QString::number(fileInfo.size()));
        jsonArray.append(fileJsonObj);
    }
    parentJsonObj["nodes"] = jsonArray;

    return file_list;
}

void Document::reloadDocument()
{
    // 解析并渲染目标文档
    if (m_mdFilePath.isEmpty()){
        qDebug() << "m_mdFilePath is empty!! " << m_mdFilePath;
        return;
    }
    qDebug() << "m_mdFilePath: " << m_mdFilePath;

    // 不同 QT 版本调用不同 Markdown 渲染方法:
    // QT_VERSION >= 5.14 QT 原生渲染函数
    // QT_VERSION < 5.14     自解析渲染函数
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    m_ui->textBrowser->setSource(m_mdFilePath);

#else
    QString hStr = mdFile2HtmlStr(m_mdFilePath);
    m_ui->textBrowser->setHtml(hStr);
#endif
}

void Document::searchKeyword()
{
    QString keyword = m_ui->lineEditKeyword->text();
}

void Document::onPushButtonBackHomeClicked()
{
    emit backHomeClicked("HOME");
}

void Document::onTextBrowserBackwardAvailable(bool arg1)
{
    m_ui->pushButtonBackward->setEnabled(arg1);
}

void Document::onTextBrowserForwardAvailable(bool arg1)
{
    m_ui->pushButtonForward->setEnabled(arg1);
}
