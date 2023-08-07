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
#include <QFileInfoList>
#include <QLineEdit>
#include <QTextCursor>
#include <QTreeWidgetItem>
#include <QWidget>

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
    void showTOC(QTreeWidgetItem *rootItem, const QJsonObject& obj, int level = 0);
    // 解析 Markdown 文档为 HTML 字符串
    QString mdFile2HtmlStr(const QString &mdPath);
    // 重新渲染文档
    void reloadDocument();
    void renderCatalog(QJsonObject& jsonObject);
    // 设置搜索命中样式
    void setMatchStyle(QTextCursor& cursor);
    // 清除搜索命中样式
    void unsetMatchStyle(QTextCursor& cursor);

private slots:
    void tocItemScrollToAnchor(QTreeWidgetItem *item, int column);
    void backHome();

public slots:
    void searchNextKeyword(const QString& keyword);
    void searchPrevKeyword(const QString& keyword);
    void searchKeywordClose(const QString& keyword);
    void searchKeywordChange(const QString& keyword);
    // 清除搜索项的高亮颜色
    void clearSearchHighlights(const QString& keyword);
    void clearSearchHighlights();
    // 处理 a 标签的点击事件，用于文档之间的跳转
    void openDocumentURL(const QUrl& url);

signals:
    void backHomeClicked(const QString& key);
    void keywordCountDone(int sum, int index);

private:
    Ui::Document *m_ui;

    // 解析后的 HTML 字符串
    QString m_htmlStr;
    QLineEdit *lineEditKeyword{};
    // 显示文档目录的控件
    QTreeWidget *m_treeWidget{};
    // 记录上一次搜索匹配项的位置
    QTextCursor m_lastMatch;
    // 用于存储所有匹配项的位置
    QList<QTextCursor> m_matchList;
    int m_matchIndex = 0;
    // 首个 QTreeWidget 是否选中
    bool m_firstItemSelected = false;
    // 搜索初始化
    bool m_initSearched = false;
    // 初始化视图
    void init();
    // HTML 字符串保存到文件
    static void htmlStrSaveToFile(QString& fileName, QString& hStr);
    void fillMatchList(const QString& searchText);
};
