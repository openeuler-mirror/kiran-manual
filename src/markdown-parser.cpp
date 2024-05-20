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

#include "markdown-parser.h"
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <codecvt>
#include <regex>
#include "constants.h"

void removeEscapeChar(char* str);
void removeEscapeChar(string& str);

namespace Kiran
{
MarkdownParser::MarkdownParser(QObject* parent)
    : QObject(parent)
{
}

void removeEscapeChar(char* str)
{
    regex re(R"([\r\n]+$)");
    string s = string(str);
    s = regex_replace(s, re, std::string(""));
    strcpy(str, s.c_str());
}

void removeEscapeChar(string& str)
{
    regex re(R"([\r\n]+$)");
    str = regex_replace(str, re, std::string(""));
}

void MarkdownParser::transfer()
{
    // 若不是 .md 文档，直接退出
    string suffix = ".md";
    if (m_filename.length() < suffix.length() &&
        m_filename.substr(m_filename.length() - suffix.length()) != suffix)
    {
        return;
    }
    //打开文件
    ifstream fin(m_filename);
    if (!fin.is_open())
    {
        cerr << "打开文件失败" << endl;
        return;
    }

    bool inBlock = false;
    int cntTag = 0;
    //读取内容(按行读取)
    string rowStr;
    while (fin.peek() != EOF)
    {
        getline(fin, rowStr);

        //预处理:处理行首空格
        const char* start = processStr(rowStr.c_str());

        // 忽略空内容
        if (!inBlock && start == nullptr)
        {
            m_root->_child.push_back(new Node(HTML_TOKEN_BLANK_ROW));
            continue;
        }
        // 代码块中空行,直接处理,不过类型判定
        if (inBlock && start == nullptr)
        {
            m_root->_child.back()->elem[0] += rowStr;
            m_root->_child.back()->elem[0] += '\n';
            continue;
        }

        //判断是否为水平分割线
        if (!inBlock && isCutLine(start))
        {
            m_root->_child.push_back(new Node(HTML_TOKEN_HR));
            continue;
        }

        //语法解析//字典
        pair<int, const char*> typeRet = parseType(start);

        //创建语法结点
        //代码块结点
        if (typeRet.first == HTML_TOKEN_BLOCK_CODE)
        {
            //判断代码块起始还是结束
            if (!inBlock)
            {
                //代码块起始
                //创建代码块结点
                m_root->_child.push_back(new Node(HTML_TOKEN_BLOCK_CODE));
            }

            //如果是代码块结束位置,不需要创建新的代码块结点

            inBlock = !inBlock;
            continue;
        }

        //判断是否为代码块中代码
        if (inBlock)
        {
            // 处理 <> 否则会被 HTML 渲染为tag 同一行中存在 <> 则处理
            if (rowStr.find('<') && rowStr.find('>'))
            {
                std::regex re("<");
                std::string result = std::regex_replace(rowStr, re, std::string("&lt;"));
                re = std::regex(">");
                rowStr = std::regex_replace(result, re, std::string("&gt;"));
            }
            // 行首主动添加空格
            m_root->_child.back()->elem[0] += "&nbsp;&nbsp;";
            m_root->_child.back()->elem[0] += rowStr;
            m_root->_child.back()->elem[0] += '\n';
            continue;
        }

        //段落结点
        if (typeRet.first == HTML_TOKEN_PARAGRAPH)
        {
            //创建一个段落结点
            m_root->_child.push_back(new Node(HTML_TOKEN_PARAGRAPH));
            //逐字符插入
            insert(m_root->_child.back(), typeRet.second);
            continue;
        }

        //标题结点
        if (typeRet.first >= HTML_TOKEN_H1 && typeRet.first <= HTML_TOKEN_H6)
        {
            //创建标题结点
            m_root->_child.push_back(new Node(typeRet.first));
            //插入标题内容
            m_root->_child.back()->elem[0] = typeRet.second;
            // 插入目录
            char* hd = const_cast<char*>(typeRet.second);
            removeEscapeChar(hd);
            cins(m_croot, typeRet.first - HTML_TOKEN_H1 + 1, hd, cntTag);
            continue;
        }

        //无序列表
        if (typeRet.first == HTML_TOKEN_UL)
        {
            //判断是否为无序列表的第一项
            //文档开始,或者语法书最后一个结点不是无序列表结点
            if (m_root->_child.empty() || m_root->_child.back()->type != HTML_TOKEN_UL)
            {
                //创建无序列表
                m_root->_child.push_back(new Node(HTML_TOKEN_UL));
            }
            //给无序列表加入列表子节点
            Node* uNode = m_root->_child.back();
            uNode->_child.push_back(new Node(HTML_TOKEN_LI));
            //给列表子节点插入内容
            insert(uNode->_child.back(), typeRet.second);
            continue;
        }

        //有序列表
        if (typeRet.first == HTML_TOKEN_OL)
        {
            if (m_root->_child.empty() || m_root->_child.back()->type != HTML_TOKEN_OL)
            {
                m_root->_child.push_back(new Node(HTML_TOKEN_OL));
            }
            Node* oNode = m_root->_child.back();
            oNode->_child.push_back(new Node(HTML_TOKEN_LI));
            insert(oNode->_child.back(), typeRet.second);
            continue;
        }

        //引用
        if (typeRet.first == HTML_TOKEN_QUOTE)
        {
            //创建引用结点
            m_root->_child.push_back(new Node(HTML_TOKEN_QUOTE));
            insert(m_root->_child.back(), typeRet.second);
        }
    }
    //展开语法树,生成HTML文档
    dfs(m_root);
    // 构造目录
    m_toc += "<ul>";
    for (int i = 0; i < (int)m_croot->_child.size(); i++)
    {
        cdfs(m_croot->_child[i], to_string(i + 1) + ".");
    }
    m_toc += "</ul>";

    // 构建目录 JSON
    buildJSONTOC();
}

QJsonObject MarkdownParser::buildJSONTOC()
{
    // 构造 JSON
    QJsonObject rootObject;
    QJsonArray baseArray;
    for (int i = 0; i < (int)m_croot->_child.size(); i++)
    {
        QJsonObject baseItemObject;
        baseItemObject.insert("heading", QString::fromStdString(m_croot->_child[i]->heading));
        QJsonArray baseItemArray;
        cdfsForJson(m_croot->_child[i], to_string(i + 1) + ".", baseItemArray);
        baseItemObject.insert("child", baseItemArray);

        baseArray.append(baseItemObject);
    }
    rootObject["_child"] = baseArray;
    return rootObject;
}

void MarkdownParser::cdfsForJson(CatalogNode* v, const string& index, QJsonArray& jsonArray)
{
    int n = (int)v->_child.size();
    if (n)
    {
        for (int i = 0; i < n; i++)
        {
            QJsonObject itemObject;
            itemObject.insert("heading", QString::fromStdString(v->_child[i]->heading));
            QJsonArray itemArray;
            cdfsForJson(v->_child[i], index + to_string(i + 1) + ".", itemArray);
            itemObject.insert("child", itemArray);

            jsonArray.append(itemObject);
        }
    }
}

void MarkdownParser::dfs(Node* root)
{
    // 此段只处理前置标签
    // h1~h6 标题增加锚点
    if (root->type >= HTML_TOKEN_H1 && root->type <= HTML_TOKEN_H6)
    {
        m_content += frontTag[root->type] + " id=\"";
        string idStr = root->elem[0];
        removeEscapeChar(idStr);
        m_content += idStr;
        m_content += "\">";
    }
    else
    {
        //插入前置标签
        m_content += frontTag[root->type];
    }

    //插入内容
    //网址
    if (root->type == HTML_TOKEN_HREF)
    {
        m_content += "<a href=\"";
        m_content += root->elem[1];
        m_content += "\">";
        m_content += root->elem[0];
        m_content += "</a>";
    }
    //图片
    else if (root->type == HTML_TOKEN_IMAGE)
    {
        const std::string prePath = IMAGE_FOR_DOCS_FOLDER;
        const std::string img = prePath + root->elem[1];

        QImage image(QString::fromStdString(img));
        int width = qMin(image.width(), DOC_IMAGE_MAX_WIDTH);

        m_content += "<img width=\"" + std::to_string(width) + "\" alt=\"";
        m_content += root->elem[0];
        m_content += "\" src=\"";
        m_content += img;
        m_content += "\" />";
    }
    // 处理空行
    else if (root->type == HTML_TOKEN_BLANK_ROW)
    {
        m_content += "\n";
    }
    //其他
    else
    {
        m_content += root->elem[0];
    }

    // 处理孩子结点
    for (Node* ch : root->_child)
    {
        dfs(ch);
    }

    //插入后置标签
    m_content += backTag[root->type];
}

const char* MarkdownParser::processStr(const char* str)
{
    while (*str)
    {
        if (*str == ' ' || *str == '\t')
        {
            ++str;
        }
        else
        {
            break;
        }
    }
    if (*str == '\0')
    {
        return nullptr;
    }
    return str;
}

void MarkdownParser::insert(Node* curNode, const char* str)
{
    bool inCode = false;    //是否为行内代码
    bool inStrong = false;  //粗体
    bool inEm = false;      //斜体
    int len = strlen(str);
    //解析内容为纯文本,可以放入纯文本结点中
    //先创建一个纯文本孩子结点
    curNode->_child.push_back(new Node(HTML_TOKEN_NUL));
    for (int i = 0; i < len; ++i)
    {
        //1.行内代码
        if (str[i] == '`')
        {
            if (inCode)
            {
                // 第二次遇到 ` 表示行内代码结束,则创建一个新的孩子结点,存放后序内容
                curNode->_child.push_back(new Node(HTML_TOKEN_NUL));
            }
            else
            {
                //创建行内代码
                curNode->_child.push_back(new Node(HTML_TOKEN_CODE));
            }
            inCode = !inCode;
            continue;
        }

        //2.粗体: "**  **"
        //出现"**"且不在行内代码中
        if (str[i] == '*' && i + 1 < len && str[i + 1] == '*' && !inCode)
        {
            if (inStrong)
            {
                //粗体结束,创建新结点
                curNode->_child.push_back(new Node(HTML_TOKEN_NUL));
            }
            else
            {
                //创建新的粗体结点
                curNode->_child.push_back(new Node(HTML_TOKEN_STRONG));
            }

            inStrong = !inStrong;
            //跳过粗体语法
            ++i;
            continue;
        }

        //3.斜体:	"_   _"
        if (str[i] == '_' && !inCode)
        {
            if (inEm)
            {
                curNode->_child.push_back(new Node(HTML_TOKEN_NUL));
            }
            else
            {
                curNode->_child.push_back(new Node(HTML_TOKEN_EM));
            }
            inEm = !inEm;
            continue;
        }

        //4.图片:	![图片名称](图片地址)
        if (str[i] == '!' && i + 1 < len && str[i + 1] == '[')
        {
            //创建图片结点
            curNode->_child.push_back(new Node(HTML_TOKEN_IMAGE));
            Node* iNode = curNode->_child.back();
            i += 2;
            //存放图片名称在elem[0]
            for (; i < len && str[i] != ']'; ++i)
            {
                iNode->elem[0] += str[i];
            }
            //存放图片地址在elem[1]
            i += 2;
            for (; i < len && str[i] != ')'; ++i)
            {
                iNode->elem[1] += str[i];
            }

            //创建新结点处理后续内容
            curNode->_child.push_back(new Node(HTML_TOKEN_NUL));
            continue;
        }

        //5.链接:[链接名](网址)
        //有左括号,且不在行内代码
        if (str[i] == '[' && !inCode)
        {
            //创建链接结点
            curNode->_child.push_back(new Node(HTML_TOKEN_HREF));
            Node* hNode = curNode->_child.back();
            ++i;
            //存放链接名在elem[0]
            for (; i < len && str[i] != ']'; ++i)
            {
                hNode->elem[0] += str[i];
            }
            //存放链接地址在elem[1]
            i += 2;
            for (; i < len && str[i] != ')'; ++i)
            {
                hNode->elem[1] += str[i];
            }
            //创建新结点处理后续内容
            curNode->_child.push_back(new Node(HTML_TOKEN_NUL));
            continue;
        }

        //6.普通纯文本
        curNode->_child.back()->elem[0] += str[i];
    }
}

pair<int, const char*> MarkdownParser::parseType(const char* str)
{
    //解析标题:# + 空格
    const char* ptr = str;
    int titleNum = 0;
    while (*ptr && *ptr == '#')
    {
        ++ptr;
        ++titleNum;
    }
    //1.标题 (# 号后面为空格)
    if (*ptr == ' ' && titleNum > 0 && titleNum <= 6)
    {
        return make_pair(HTML_TOKEN_H1 + titleNum - 1, ptr + 1);
    }
    //不符合标题语法,需要重新解析
    ptr = str;

    //2.代码块:```代码内容```
    if (strncmp(ptr, "```", 3) == 0)
    {
        return make_pair(HTML_TOKEN_BLOCK_CODE, ptr + 3);
    }

    //3.无序列表: - + 空格
    if (strncmp(ptr, "- ", 2) == 0 || strncmp(ptr, "+ ", 2) == 0)
    {
        return make_pair(HTML_TOKEN_UL, ptr + 2);
    }

    //4.有序列表:数字字符 + "." + 空格
    if (*ptr >= '0' && *ptr <= '9')
    {
        //遍历完数字
        while (*ptr && (*ptr >= '0' && *ptr <= '9')) ++ptr;
        if (*ptr && *ptr == '.')
        {
            ++ptr;
            if (*ptr && *ptr == ' ')
                return make_pair(HTML_TOKEN_OL, ptr + 1);
        }
    }
    //重置
    ptr = str;
    //5.引用:">" + 空格
    if (strncmp(ptr, "> ", 2) == 0)
    {
        return make_pair(HTML_TOKEN_QUOTE, ptr + 2);
    }

    //其他语法统一解析为段落
    return make_pair(HTML_TOKEN_PARAGRAPH, str);
}

bool MarkdownParser::isCutLine(const char* str)
{
    int cnt = 0;
    while (*str && *str == '-')
    {
        ++str;
        ++cnt;
    }
    return cnt >= 3;
}

string MarkdownParser::html()
{
    std::string head =
        "<!DOCTYPE html><html><head>\
        <meta charset=\"utf-8\">\
        <title>Markdown</title>\
        <style>\
                h1 { font-weight: 400; }\
                h2 { font-weight: 400; }\
                h3 { font-weight: 400; }\
                h4 { font-weight: 400; }\
                h5 { font-weight: 400; }\
                h6 { font-weight: 400; }\
                strong { font-weight: 500 }\
                ul, ol { font-weight: 400; }\
		pre { background-color: #cccccc;padding-left:100px; white-space: pre-wrap; margin-right: 5px}\
		pre>code { color: black; background-color: #cccccc;text-intend:100px}\
        </style>\
        </head><body><article class=\"markdown-body\">";
    std::string end = "</article></body></html>";

    return head + m_content + end;
}

void MarkdownParser::cins(CatalogNode* v, int x, const string& hd, int tag)
{
    int n = (int)v->_child.size();
    if (x == 1)
    {
        v->_child.push_back(new CatalogNode(hd));
        v->_child.back()->tag = "tag" + to_string(tag);
        return;
    }

    if (!n || v->_child.back()->heading.empty())
        v->_child.push_back(new CatalogNode(""));
    cins(v->_child.back(), x - 1, hd, tag);
}

void MarkdownParser::cdfs(CatalogNode* v, const string& index)
{
    m_toc += "<li>";
    m_toc += "<a href=\"#" + v->heading + "\">" + v->heading + "</a>";
    int n = (int)v->_child.size();
    if (n)
    {
        m_toc += "<ul>";
        for (int i = 0; i < n; i++)
        {
            cdfs(v->_child[i], index + to_string(i + 1) + ".");
        }
        m_toc += "</ul>";
    }
    m_toc += "</li>";
}

void MarkdownParser::destory(Node* root)
{
    if (root)
    {
        for (Node* ch : root->_child)
        {
            destory(ch);
        }

        delete root;
    }
}
}  // namespace Kiran