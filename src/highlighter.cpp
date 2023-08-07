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

#include "highlighter.h"
#include <QTextBlockFormat>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    // 定义关键字规则，下列为常见 C++ 关键字
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b"
                    << "\\bclass\\b"
                    << "\\bconst\\b"
                    << "\\bdouble\\b"
                    << "\\benum\\b"
                    << "\\bexplicit\\b"
                    << "\\bfriend\\b"
                    << "\\binline\\b"
                    << "\\bint\\b"
                    << "\\blong\\b"
                    << "\\bnamespace\\b"
                    << "\\boperator\\b"
                    << "\\bprivate\\b"
                    << "\\bprotected\\b"
                    << "\\bpublic\\b"
                    << "\\bshort\\b"
                    << "\\bsignals\\b"
                    << "\\bsigned\\b"
                    << "\\bslots\\b"
                    << "\\bstatic\\b"
                    << "\\bstruct\\b"
                    << "\\btemplate\\b"
                    << "\\btypedef\\b"
                    << "\\btypename\\b"
                    << "\\bunion\\b"
                    << "\\bunsigned\\b"
                    << "\\bvirtual\\b"
                    << "\\bvoid\\b"
                    << "\\bvolatile\\b"
                    << "\\bbool\\b";
    foreach (const QString &pattern, keywordPatterns)
    {
        // 定义 pattern
        rule.pattern = QRegularExpression(pattern);
        // 定义 format
        rule.format = keywordFormat;
        // 加入规则集合
        highlightingRules.append(rule);
    }
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    // 单行注释
    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    //    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");

    codeBlockFormat.setForeground(Qt::yellow);
    codeBlockStartExpression = QRegularExpression("<code>");
    codeBlockEndExpression = QRegularExpression("</code>");
}
void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0)
    {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
    // TODO: 实现只对 <code> 块中的代码进行高亮匹配
    setCurrentBlockState(0);
    startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(codeBlockStartExpression);
    while (startIndex >= 0)
    {
        QRegularExpressionMatch match = codeBlockEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, codeBlockFormat);
        startIndex = text.indexOf(codeBlockStartExpression, startIndex + commentLength);
    }
}
