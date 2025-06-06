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
#include "code-highlighter.h"
#include "ui_document.h"

#include <kiran-log/qt5-log-i.h>
#include <kiranwidgets-qt5/kiran-message-box.h>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QGraphicsDropShadowEffect>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <QScrollBar>
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include <string>
#include <palette.h>
#include "markdown-parser.h"
#include <QAbstractTextDocumentLayout>

using namespace Kiran::Theme;

namespace Kiran
{
Document::Document(QWidget* parent)
    : QWidget(parent),
      m_ui(new Ui::Document)
{
    m_ui->setupUi(this);
    init();
}

Document::~Document()
{
    delete m_ui;
}

QString Document::mdFile2HtmlStr(const QString& mdPath)
{
    using namespace std;
    string mdFilePath = string((const char*)mdPath.toLocal8Bit());
    Kiran::MarkdownParser markdownParser(mdFilePath);
    markdownParser.transfer();
    string htmlStr = markdownParser.html();

    QJsonObject rootObject = markdownParser.buildJSONTOC();
    renderCatalog(rootObject);
    return QString(QString::fromLocal8Bit(htmlStr.c_str()));
}

void Document::renderDocument(const QString& mdFilePath)
{
    this->setMdFilePath(mdFilePath);
    this->renderDocument();
}

void Document::renderCatalog(QJsonObject& jsonObject)
{
    // 获取目录到 JSON 格式
    // 重置 QTreeWidget 状态
    m_ui->treeWidget->clear();
    if (m_matchIndex == m_matchList.size())
        this->m_firstItemSelected = false;

    QJsonArray jsonArray = jsonObject["_child"].toArray();
    for (auto obj : jsonArray)
    {
        if (obj.isObject())
        {
            showTOC(nullptr, obj.toObject(), 0);
        }
    }
    m_ui->treeWidget->expandAll();
}

void Document::showTOC(QTreeWidgetItem* root, const QJsonObject& obj, int level)
{
    QTreeWidgetItem* childRoot;
    // 输出当前对象的 heading 属性
    if (obj.contains("heading"))
    {
        QJsonValue headingValue = obj.value("heading");
        if (headingValue.isString())
        {
            QString heading = headingValue.toString();
            childRoot = new QTreeWidgetItem(QStringList() << heading);
            // 设置样式
            if (root == nullptr)
            {
                m_ui->treeWidget->addTopLevelItem(childRoot);
                if (level == 0 && !m_firstItemSelected)
                {
                    childRoot->setSelected(true);
                    this->m_firstItemSelected = true;
                }
            }
            else
            {
                root->addChild(childRoot);
            }
        }
    }
    // 递归遍历当前对象的 child 数组
    if (obj.contains("child"))
    {
        QJsonValue childValue = obj.value("child");
        if (childValue.isArray())
        {
            QJsonArray childArray = childValue.toArray();
            for (auto child : childArray)
            {
                if (child.isObject())
                {
                    showTOC(childRoot, child.toObject(), level + 1);
                }
            }
        }
    }
}

void Document::setMatchStyle(QTextCursor& cursor)
{
    QTextCharFormat format;
    format.setBackground(Qt::yellow);
    cursor.mergeCharFormat(format);
}

void Document::unsetMatchStyle(QTextCursor& cursor)
{
    QTextCharFormat format;
    format.setBackground(Qt::transparent);
    cursor.mergeCharFormat(format);
}

void Document::tocItemScrollToAnchor(QTreeWidgetItem* item, int column)
{
    QString itemName = item->text(column);
    m_ui->textBrowser->scrollToAnchor(itemName);
}

void Document::backToNavigationPage()
{
    emit backHomeClicked("HOME");
}

void Document::searchNextKeyword(const QString& keyword)
{
    if (keyword.trimmed().isEmpty())
    {
        return;
    }
    else
    {
        if (!m_initSearched)
        {
            this->fillMatchList(keyword);
            m_initSearched = true;
            if (m_matchList.empty())
            {
                KiranMessageBox::message(this,
                                         tr("No search result"),
                                         tr("Number of Keyword Matches: %1 .").arg(m_matchList.size()),
                                         KiranMessageBox::Yes);
                return;
            }
        }
        if (m_matchIndex < m_matchList.size())
        {
            // 将光标定位到匹配项的位置
            emit keywordCountDone(m_matchList.size(), m_matchIndex + 1);
            m_ui->textBrowser->setTextCursor(m_matchList.at(m_matchIndex));
            m_ui->textBrowser->ensureCursorVisible();
            m_matchIndex++;
        }
        else if (m_matchIndex == m_matchList.size())
        {
            m_matchIndex = 0;
            // 将光标定位到匹配项的位置
            emit keywordCountDone(m_matchList.size(), m_matchIndex + 1);
            m_ui->textBrowser->setTextCursor(m_matchList.at(m_matchIndex));
            m_ui->textBrowser->ensureCursorVisible();
            m_matchIndex++;
        }
    }
}
void Document::searchPrevKeyword(const QString& keyword)
{
    if (m_matchList.size() < 2)
    {
        return;
    }
    --m_matchIndex;
    if (m_matchIndex < 0)
    {
        m_matchIndex = m_matchList.size() - 1;
    }
    QTextCursor prevCursor = m_matchList.at(m_matchIndex);
    // 将光标定位到匹配项的位置
    emit keywordCountDone(m_matchList.size(), m_matchIndex + 1);
    m_ui->textBrowser->setTextCursor(prevCursor);
    m_ui->textBrowser->ensureCursorVisible();
}

void Document::searchKeywordClose(const QString& keyword)
{
    this->m_initSearched = false;
    this->m_matchIndex = 0;
    this->clearSearchHighlights();
}

void Document::searchKeywordChange(const QString& keyword)
{
    this->m_initSearched = false;
    this->m_matchIndex = 0;
    this->clearSearchHighlights();
    if (keyword.isEmpty() || keyword.isNull())
    {
    }
}

void Document::clearSearchHighlights()
{
    for (QTextCursor cursor : m_matchList)
    {
        this->unsetMatchStyle(cursor);
    }
    //    this->m_ui->textBrowser->moveCursor(QTextCursor::Start);
    this->m_matchList.clear();
}

void Document::clearSearchHighlights(const QString& keyword)
{
    m_lastMatch = QTextCursor();
    m_matchList.clear();
    m_matchIndex = 0;
    QTextDocument* document = m_ui->textBrowser->document();
    QTextCursor cursor(document);

    while (!cursor.isNull() && !cursor.atEnd())
    {
        cursor = document->find(keyword, cursor, QTextDocument::FindWholeWords);
        if (!cursor.isNull())
        {
            this->unsetMatchStyle(cursor);
        }
    }
    this->m_matchList.clear();
}

void Document::openDocumentURL(const QUrl& url)
{
    QString strUrl = url.toString();
    int pos = 0;
    // 首先判断是否是超链接
    QRegExp rx;
    rx.setPattern("(https?|ftp|file)://[-A-Za-z0-9+&@#/%?=~_|!:,.;]+[-A-Za-z0-9+&@#/%=~_|]");
    QRegExpValidator rv;
    rv.setRegExp(rx);
    QValidator::State rvState = rv.validate(strUrl, pos);
    if (rvState == QValidator::Acceptable)
    {
        auto result = KiranMessageBox::message(this, tr("Notice!"), tr("About to open the Browser and go to: %1").arg(strUrl), KiranMessageBox::Ok | KiranMessageBox::No);
        if (result == KiranMessageBox::Ok)
        {
            QDesktopServices::openUrl(url);
        }
        return;
    }
    // 获取当前文档父路径
    QRegularExpression docNameRegex("(.*/)[^/]*$");
    QRegularExpressionMatch match = docNameRegex.match(this->m_mdFilePath);
    // 获取目标完整路径
    QString targetUrl{};
    if (match.hasMatch())
    {
        QString dirPath = match.captured(1);
        targetUrl = dirPath + strUrl;
    }
    if (targetUrl.isEmpty() || !QFile::exists(targetUrl))
    {
        KiranMessageBox::message(this, tr("Notice!"), tr("Target document does not exist! \nDocument Name: ") + strUrl, KiranMessageBox::Cancel);
    }
    else
    {
        this->renderDocument(targetUrl);
    }
}

void Document::init()
{
    auto outLayout = new QVBoxLayout(this);
    outLayout->setMargin(0);
    // 组件初始化
    auto highlighter = new CodeHighlighter(m_ui->textBrowser->document());
    Q_UNUSED(highlighter);

    connect(m_ui->treeWidget, &QTreeWidget::itemClicked, this, &Document::tocItemScrollToAnchor);
    connect(m_ui->pushButtonBackHome, &QPushButton::clicked, this, &Document::backToNavigationPage);
    connect(m_ui->textBrowser, &QTextBrowser::anchorClicked, this, &Document::openDocumentURL);

    // Fixme: 以下代码用一种不好的方式解决 pushButtonBackHome, treeWidget, textBrowser 文字不跟随主题变化到问题
    // note: 要跟随主题变化要求控件不能设置样式表，如有样式表则会导致主题样式透传失败
    auto stylePalette = DEFAULT_PALETTE();
    connect(stylePalette, &Palette::baseColorsChanged, this, [=](){
                auto kiranPalette = DEFAULT_PALETTE();

                auto qColor = kiranPalette->getColor(Palette::NORMAL,Palette::TEXT);

                QPalette palette{};
                palette.setColor(QPalette::ButtonText, qColor);
                m_ui->pushButtonBackHome->setPalette(palette);

                // pushButtonBackHome 可以通过单独的 setPalette 来跟随，但是 treeWidget, textBrowser 单独设置无效，暂不知道原因
                // 因此采用如下方式来设置
                QPalette buttonPalette = m_ui->pushButtonBackHome->palette();
                const QColor& buttonTextColor = buttonPalette.color(QPalette::ButtonText);
                QPalette followPalette{};
                followPalette.setColor(QPalette::Text, buttonTextColor);
                m_ui->textBrowser->setPalette(followPalette);
            });
    // 监听页面变化，更新目录
    // TODO: 这种方法是用关键字匹配，若目录中有重复项会导致异常 (其实若采用　AnchorAt 类似的方法拿到　Anchor 也是会有这种　BUG 的，暂采用此方法，需要写文档时避免相同的目录)
    connect(m_ui->textBrowser->verticalScrollBar(), &QScrollBar::valueChanged, this, [=]()
            {
                QTextCursor cursor = m_ui->textBrowser->cursorForPosition(QPoint(0, 0));
                QString firstVisibleLineText = m_ui->textBrowser->document()->findBlockByLineNumber(cursor.block().blockNumber()).text();
//                int firstVisibleBlock = m_ui->textBrowser->document()->documentLayout()->blockBoundingRect(cursor.block()).topLeft().y();
//                QRect rect = m_ui->textBrowser->viewport()->rect();
//                QPoint point = QPoint(10, firstVisibleBlock - rect.y());
//                QString firstVisibleLineAnchor = m_ui->textBrowser->anchorAt(point); // AnchorAt()　只能获取到　`the refer of anchor`，　也就是　a 标签对应的　link，而获取不到　h 标签的　id
                selectTreeItem(m_ui->treeWidget->invisibleRootItem(), firstVisibleLineText);
            });
}

void Document::renderDocument()
{
    if (m_mdFilePath.isEmpty() || !QFile::exists(m_mdFilePath))
    {
        KLOG_ERROR() << "m_mdFilePath value is empty or related file not found. m_mdFilePath=" << m_mdFilePath;
        return;
    }
    m_ui->textBrowser->setHtml(mdFile2HtmlStr(m_mdFilePath));
}

void Document::htmlStrSaveToFile(QString& fileName, QString& hStr)
{
    if (fileName.endsWith(".md"))
    {
        QString pureFileName = fileName.split(".").first();
        QString htmlFilePath = "html/" + pureFileName + ".html";
        QDir().mkpath("html");
        QFile hFile(htmlFilePath);
        if (!hFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            KLOG_ERROR() << "Open File Failed: " + htmlFilePath;
            return;
        }
        QTextStream tsOut(&hFile);
        tsOut << hStr;
        KLOG_INFO() << htmlFilePath + " => HTML document save success";
    }
}

void Document::fillMatchList(const QString& searchText)
{
    QTextDocument* document = m_ui->textBrowser->document();
    QTextCursor cursor(document);

    // 查找所有匹配项
    while (!cursor.isNull() && !cursor.atEnd())
    {
        cursor = document->find(searchText, cursor);
        if (!cursor.isNull())
        {
            // 高亮
            this->setMatchStyle(cursor);
            // 将匹配项的位置添加到列表中
            m_matchList.append(cursor);
        }
    }
}
void Document::setMdFilePath(const QString& mdFilePath)
{
    this->m_mdFilePath = mdFilePath;
}
QString Document::getMdFilePath()
{
    return this->m_mdFilePath;
}
void Document::selectTreeItem(QTreeWidgetItem* item, const QString& key)
{
    if (item->text(0) == key) {
        m_ui->treeWidget->setCurrentItem(item);
        return;
    }
    // 遍历子项
    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem* childItem = item->child(i);
        selectTreeItem(childItem, key);
    }
}
}  // namespace Kiran
