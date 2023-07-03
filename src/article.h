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
#include <QWidget>
#include <QTreeWidgetItem>
#include <QFileInfoList>

namespace Ui {
class Article;
}

class Article : public QWidget
{
    Q_OBJECT

public:
    explicit Article(QWidget *parent = nullptr);
    ~Article();

    QString m_mdFilePath;     // 要渲染的 Markdown 文档路径
    QFileInfoList showDirTree(QTreeWidgetItem *root, const QString &path, QJsonObject &parentJsonObj);  // 显示文档目录树
    QString mdFile2HtmlStr(const QString &mdPath);
    void reloadArticle();

private slots:
    void onTreeWidgetItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onPushButtonSearchClicked();
    void onPushButtonBackHomeClicked();
    void onTextBrowserBackwardAvailable(bool arg1);
    void onTextBrowserForwardAvailable(bool arg1);

signals:
    void backHome(const QString& key);

private:
    Ui::Article *ui;

    QString m_htmlStr;        // 解析后的 HTML 字符串
    QTreeWidget *m_treeWidget;  // 显示文档目录的控件

    void initView();
};
