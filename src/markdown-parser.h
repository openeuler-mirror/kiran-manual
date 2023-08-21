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

#include <QObject>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

/**
 * HTML 标签类型
 */
enum HtmlToken
{
    HTML_TOKEN_NUL = 0,
    //段落
    HTML_TOKEN_PARAGRAPH = 1,
    //超链接
    HTML_TOKEN_HREF = 2,
    //无序集
    HTML_TOKEN_UL = 3,
    //有序集
    HTML_TOKEN_OL = 4,
    //列表项目
    HTML_TOKEN_LI = 5,
    //斜体，表示强调
    HTML_TOKEN_EM = 6,
    //加粗
    HTML_TOKEN_STRONG = 7,
    //标题
    HTML_TOKEN_HR = 8,
    //图片
    HTML_TOKEN_IMAGE = 9,
    //块引用
    HTML_TOKEN_QUOTE = 10,

    //标题1-6
    HTML_TOKEN_H1 = 11,
    HTML_TOKEN_H2 = 12,
    HTML_TOKEN_H3 = 13,
    HTML_TOKEN_H4 = 14,
    HTML_TOKEN_H5 = 15,
    HTML_TOKEN_H6 = 16,

    //代码块
    HTML_TOKEN_BLOCK_CODE = 17,
    //单行代码
    HTML_TOKEN_CODE = 18,
    // 空行
    HTML_TOKEN_BLANK_ROW = 19,
};

/**
 * HTML 前置标签 自定义到标签设置为空即可
 * - 注意 code 标签加了盘古之白
 * - pre 与 code 之间手动加入了换行，原因是希望前端 code block 离顶部有一定到距离，通过样式系统(padding-top)暂时没实现
 */
// clang-format off
const std::string frontTag[] = {"", "<p>", "", "<ul>", "<ol>", "<li>", "<em>", "<strong>", "<hr color=#CCCCCC size=1 / > ", "", "<blockquote>",
                                "<h1", "<h2", "<h3", "<h4", "<h5", "<h6", "<pre>\n\n<code>", " <strong>", ""};
/**
 * HTML 后置标签
 */
const std::string backTag[] = {"", "</p>", "", "</ul>", "</ol>", "</li>", "</em>", "</strong>", "", "", "</blockquote>",
                               "</h1>", "</h2>", "</h3>", "</h4>", "</h5>", "</h6>", "</code></pre>", "</strong> ", ""};
// clang-format on

/**
 * HTML DOM 节点
 */
struct Node
{
    //语法类型
    int type;
    //孩子节点
    vector<Node*> _child;
    //内容
    //elem[0]:保存需要显示的内容
    //elem[1]:保存网址/路径
    string elem[2];

    explicit Node(int t)
    {
        type = t;
    }
};

/**
 * 目录结构
 */
struct CatalogNode
{
    vector<CatalogNode*> _child;
    string heading;
    string tag;
    CatalogNode(string hd)
        : heading(std::move(hd)) {}
};

class MarkdownParser : public QObject
{
    Q_OBJECT
public:
    explicit MarkdownParser(QObject* parent = nullptr);
    explicit MarkdownParser(const string& filename)
    {
        m_root = new Node(HTML_TOKEN_NUL);
        m_croot = new CatalogNode("");
        m_filename = filename;
    }

    ~MarkdownParser() override
    {
        if (m_root)
        {
            destory(m_root);
        }
    }

    // 打开文件读取数据
    void transfer();
    // 语法树转换成HTML源代码(DFS)
    void dfs(Node* root);
    // 去除行首空格
    static const char* processStr(const char* str);
    // 逐字符内容插入
    static void insert(Node* curNode, const char* str);
    // 解析行首语法  返回:语法类型 + 对应内容起始位置
    static pair<int, const char*> parseType(const char* str);
    // 打开文件读取数据判断水平分割线 "---"
    static bool isCutLine(const char* str);
    // 生成 HTML 文档。
    string html();
    // 销毁节点
    void destory(Node* root);
    // 插入目录节点
    void cins(CatalogNode* v, int x, const string& hd, int tag);
    // 遍历目录节点
    void cdfs(CatalogNode* v, const string& index);
    // 构建目录节点的 JSON 结构
    // 此数据结构用于与目录渲染模块交互
    QJsonObject buildJSONTOC();
    void cdfsForJson(CatalogNode* v, const string& index, QJsonArray& jsonArray);

private:
    //语法树根节点
    Node* m_root{};
    // 目录节点
    CatalogNode* m_croot{};
    //文件名
    string m_filename;
    //存放HTML文档内容
    string m_content;
    // 存放目录
    string m_toc;
};
