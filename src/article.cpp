#include "article-page.h"
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
#include "ui_articlepage.h"

ArticlePage::ArticlePage(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ArticlePage)
{
    ui->setupUi(this);
    initView();
}

ArticlePage::~ArticlePage()
{
    delete ui;
}

void ArticlePage::initView()
{
    // 组件初始化
    ui->textBrowser->setOpenExternalLinks(true);        //设置 QTextBrowser 能自动用系统浏览器打开外站链接
    ui->pushButtonBackward->setEnabled(false);          //将 "后退"、"前进"按钮设置为不可用状态
    ui->pushButtonForward->setEnabled(false);           //将 "后退"、"前进"按钮设置为不可用状态
    connect(ui->pushButtonBackward, SIGNAL(clicked()),  // 前进后退按钮关联到 TextBrowser 槽函数
            ui->textBrowser,
            SLOT(backward()));
    connect(ui->pushButtonForward, SIGNAL(clicked()), ui->textBrowser, SLOT(forward()));

    QString dirPath = "/home/skyzcyou/Documents/manual_book/";
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidget);
    //    ui->treeWidget->setItemWidget(root,2,nullptr);
    root->setText(0, "manual_book");
    QJsonObject rootJsonObj;
    ShowDirTree(root, dirPath, rootJsonObj);

    //    Pretreatment pretreatment(nullptr);
    //    QFileInfoList lists = pretreatment.allfile(dirPath, rootJsonObj);
    //    LoadJson("/home/skyzcyou/Documents/manual_tree.json");
}

QString ArticlePage::mdFile2HtmlStr(QString mdPath)
{
    using namespace std;
    string mp = string((const char *)mdPath.toLocal8Bit());
    MarkdownParser mk(mp);
    mk.transferm();
    string mkStr = mk.html();
    return QString(QString::fromLocal8Bit(mkStr.c_str()));
}

void ArticlePage::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
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
QFileInfoList ArticlePage::ShowDirTree(QTreeWidgetItem *root, QString path, QJsonObject &parentJsonObj)
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
        QFileInfoList child_file_list = ShowDirTree(childroot, namepath, subdirJsonObj);
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

void ArticlePage::on_pushButtonSearch_clicked()
{
    QString keyword = ui->lineEditKeyword->text();
}

void ArticlePage::on_textBrowser_backwardAvailable(bool arg1)
{
    ui->pushButtonBackward->setEnabled(arg1);
}

void ArticlePage::on_textBrowser_forwardAvailable(bool arg1)
{
    ui->pushButtonForward->setEnabled(arg1);
}
