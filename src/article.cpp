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

#include "article.h"
#include "ui_article.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include <QtDebug>
#include <string>
#include "markdown-parser.h"
#include "search.h"


Article::Article(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Article)
{
    ui->setupUi(this);
    initView();

}

Article::~Article()
{
    delete ui;
}

void Article::initView()
{
    // 组件初始化
    ui->textBrowser->setOpenExternalLinks(true);
    ui->pushButtonBackward->setEnabled(false);
    ui->pushButtonForward->setEnabled(false);
    // 关联到槽函数
    connect(ui->pushButtonBackward, &QPushButton::clicked, ui->textBrowser,&QTextBrowser::backward);
    connect(ui->pushButtonForward, &QPushButton::clicked, ui->textBrowser, &QTextBrowser::forward);
    connect(ui->treeWidget, &QTreeWidget::itemDoubleClicked, this, &Article::onTreeWidgetItemDoubleClicked);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Article::onPushButtonSearchClicked);
    connect(ui->pushButtonBackHome, &QPushButton::clicked, this, &Article::onPushButtonBackHomeClicked);
    connect(ui->textBrowser, &QTextBrowser::forwardAvailable, this, &Article::onTextBrowserForwardAvailable);
    connect(ui->textBrowser, &QTextBrowser::backwardAvailable, this, &Article::onTextBrowserBackwardAvailable);

    QString dirPath = "/home/skyzcyou/Documents/manual_book/";
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidget);
    root->setText(0, "manual_book");
    QJsonObject rootJsonObj;
    showDirTree(root, dirPath, rootJsonObj);

}
/**
 * @brief Article::mdFile2HtmlStr
 * @param mdPath: Markdown 文档路径
 * @return QString htmlStr: 返回解析 Markdown 成功后的 HTML 字符串
 */
QString Article::mdFile2HtmlStr(const QString& mdPath)
{
    using namespace std;
    string mp = string((const char *)mdPath.toLocal8Bit());
    MarkdownParser mk(mp);
    mk.transferm();
    string mkStr = mk.html();
    return QString(QString::fromLocal8Bit(mkStr.c_str()));
}

void Article::onTreeWidgetItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString fileName = item->text(column);

    QStringList filePath;
    while (item != NULL)
    {
        filePath << item->text(0);  //获取itemfile名称
        item = item->parent();      //将itemfile指向父item
    }
    QString strpath;
    //cout<<filepath.size()<<endl;
    for (int i = (filePath.size() - 1); i >= 0; i--)  //QStringlist类filepath反向存着初始item的路径
    {                                                 //将filepath反向输出，相应的加入’/‘
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
        qDebug() << fileName + "=> HTML Docment Save Success";
    }
    qDebug() << "前进? " << ui->textBrowser->isForwardAvailable();
    qDebug() << "后退? " << ui->textBrowser->isBackwardAvailable();

    ui->textBrowser->setHtml(hStr);
}

// FIXME: 获取文档目录
QFileInfoList Article::showDirTree(QTreeWidgetItem *root, const QString &path, QJsonObject &parentJsonObj)
{
    QDir dir(path);       // Traversal subdirectory
    QDir dir_file(path);  // Traversal all file in a subdirectory

    // Traversal folder add into Widget
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (int i = 0; i != folder_list.size(); i++)
    {
        // JSON of current node
        QJsonObject subdirJsonObj;
        subdirJsonObj["type"] = QJsonValue("dir");

        QString namepath = folder_list.at(i).absoluteFilePath();  // get absolute path
        QFileInfo folderinfo = folder_list.at(i);

        QString name = folderinfo.fileName();  // get folder name

        // skip file name star with "_"
        if (name.startsWith("_"))
        {
            continue;
        }
        QTreeWidgetItem *childroot = new QTreeWidgetItem(QStringList() << name);
        root->addChild(childroot);
        childroot->setIcon(0, QIcon(path + "_images/_self/folder.png"));
        childroot->setText(0, name);

        root->addChild(childroot);
        QFileInfoList child_file_list = showDirTree(childroot, namepath, subdirJsonObj);
        file_list.append(child_file_list);
        file_list.append(name);

        subdirJsonObj["alias"] = QJsonValue(name);  // alias
        parentJsonObj[name] = subdirJsonObj;
    }

    // add file in path
    dir_file.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir_file.setSorting(QDir::Name);
    QFileInfoList list_file = dir_file.entryInfoList();

    QJsonArray jsonArray;
    for (int i = 0; i < list_file.size(); ++i)
    {
        QFileInfo fileInfo = list_file.at(i);
        QString name2 = fileInfo.fileName();
        QTreeWidgetItem *child = new QTreeWidgetItem(QStringList() << name2);
        child->setIcon(0, QIcon(path + "_images/_self/markdown.png"));
        child->setText(0, name2);
        root->addChild(child);

        QJsonObject fileJsonObj;
        fileJsonObj["name"] = QJsonValue(name2);
        fileJsonObj["alias"] = QJsonValue(name2);  // alias: TODO:set document head?
        fileJsonObj["type"] = QJsonValue("file");
        fileJsonObj["size"] = QJsonValue(QString::number(fileInfo.size()));
        jsonArray.append(fileJsonObj);
    }
    parentJsonObj["nodes"] = jsonArray;
    return file_list;
}

void Article::reloadArticle()
{
    // 解析并渲染目标文档
    if (m_mdFilePath.isEmpty()){
        qDebug() << "m_mdFilePath is empty!! " << m_mdFilePath;
        return;
    }
    qDebug() << "m_mdFilePath: " << m_mdFilePath;
    QString hStr = mdFile2HtmlStr(m_mdFilePath);
    ui->textBrowser->setHtml(hStr);
}

void Article::onPushButtonSearchClicked()
{
    QString keyword = ui->lineEditKeyword->text();
}

void Article::onPushButtonBackHomeClicked()
{
    //用 emit 发信号
    emit backHome("HOME");
}

void Article::onTextBrowserBackwardAvailable(bool arg1)
{
    ui->pushButtonBackward->setEnabled(arg1);
}

void Article::onTextBrowserForwardAvailable(bool arg1)
{
    ui->pushButtonForward->setEnabled(arg1);
}
