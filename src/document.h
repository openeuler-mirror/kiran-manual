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

#pragma once
#include <QWidget>
#include <QTreeWidgetItem>
#include <QFileInfoList>

namespace Ui {
class Document;
}

class Document : public QWidget
{
    Q_OBJECT

public:
    explicit Document(QWidget *parent = nullptr);
    ~Document() override;
    // 要渲染的 Markdown 文档路径
    QString m_mdFilePath;     
    // 显示文档目录树
    QFileInfoList showDirTree(QTreeWidgetItem *root, const QString &path, QJsonObject &parentJsonObj);  
    static QString mdFile2HtmlStr(const QString &mdPath);
    void reloadDocument();

private slots:
    void onTreeWidgetItemDoubleClicked(QTreeWidgetItem *item, int column);
    void searchKeyword();
    void onPushButtonBackHomeClicked();
    void onTextBrowserBackwardAvailable(bool arg1);
    void onTextBrowserForwardAvailable(bool arg1);

signals:
    void backHomeClicked(const QString& key);

private:
    Ui::Document *m_ui;

    // 解析后的 HTML 字符串
    QString m_htmlStr;

    // 显示文档目录的控件
    QTreeWidget *m_treeWidget{};

    void init();
};
