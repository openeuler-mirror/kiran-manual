#ifndef ARTICLEPAGE_H
#define ARTICLEPAGE_H

#include <QFileInfoList>
#include <QTreeWidgetItem>
#include <QWidget>

namespace Ui {
class ArticlePage;
}

class ArticlePage : public QWidget
{
    Q_OBJECT

public:
    explicit ArticlePage(QWidget *parent = nullptr);
    ~ArticlePage();

    QFileInfoList ShowDirTree(QTreeWidgetItem *root, QString path, QJsonObject &parentJsonObj);  // 显示文档目录树
    QString mdFile2HtmlStr(QString mdPath);

private slots:
    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_pushButtonSearch_clicked();
    void on_pushButtonBackHome_clicked();
    void on_textBrowser_backwardAvailable(bool arg1);
    void on_textBrowser_forwardAvailable(bool arg1);

signals:
    void backHome(QString key);

private:
    Ui::ArticlePage *ui;


    QString m_mdFilePath;     // 要渲染的 Markdown 文档路径
    QString m_htmlStr;        // 解析后的 HTML 字符串
    QTreeWidget *treeWidget;  // 显示文档目录的控件

    void initView();
};

#endif // ARTICLEPAGE_H
