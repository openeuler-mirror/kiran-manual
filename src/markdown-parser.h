#ifndef MARKDOWNPARSER_H
#define MARKDOWNPARSER_H

#include <QObject>
#pragma once
#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
* HTML 标签类型
*/
enum Token
{
    nul = 0,
    paragraph = 1,  //段落
    href = 2,       //超链接
    ul = 3,         //无序集
    ol = 4,         //有序集
    li = 5,         //列表项目
    em = 6,         //斜体，表示强调
    strong = 7,     //加粗
    hr = 8,         //标题
    image = 9,      //图片
    quote = 10,     //块引用

    //标题1-6
    h1 = 11,
    h2 = 12,
    h3 = 13,
    h4 = 14,
    h5 = 15,
    h6 = 16,

    blockcode = 17,  //代码块
    code = 18,       //单行代码

    space = 19,  // space
};

/**
* HTML 前置标签
*/
const std::string frontTag[] = {"", "<p>", "", "<ul>", "<ol>", "<li>", "<em>", "<strong>", "<hr color=#CCCCCC size=1 / > ", "", "<blockquote>", "<h1>", "<h2>", "<h3>", "<h4>", "<h5>", "<h6>", "<pre><code>", "<code>"};
/**
* HTML 后置标签
*/
const std::string backTag[] = {"", "</p>", "", "</ul>", "</ol>", "</li>", "</em>", "</strong>", "", "", "</blockquote>", "</h1>", "</h2>", "</h3>", "</h4>", "</h5>", "</h6>", "</code></pre>", "</code>"};

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

    Node(int t)
    {
        type = t;  //赋值
    }
};

class MarkdownParser : public QObject
{
    Q_OBJECT
public:
    explicit MarkdownParser(QObject* parent = nullptr);
    MarkdownParser(const string& filename)
    {
        m_root = new Node(nul);
        m_filename = filename;
    }
    ~MarkdownParser()
    {
        if (m_root)
        {
            destory(m_root);
        }
    }
    void transferm();                                   // 打开文件读取数据
    void dfs(Node* root);                               // 语法树转换成HTML源代码//深度优先遍历 DFS
    const char* processStr(const char* str);            // 去除行首空格
    void insert(Node* curNode, const char* str);        // 逐字符内容插入
    pair<int, const char*> parseType(const char* str);  // 解析行首语法  返回:语法类型 + 对应内容起始位置
    bool isCutLine(const char* str);                    // 打开文件读取数据判断水平分割线 "---"
    string html();
    void destory(Node* root);  // 销毁

private:
    Node* m_root;       //语法树根节点
    string m_filename;  //文件名
    string m_content;   //存放HTML文档内容
};

#endif  // MARKDOWNPARSER_H
