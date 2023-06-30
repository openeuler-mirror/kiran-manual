#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QJsonDocument>
#include <QWidget>

namespace Ui
{
class Navigation;
}

class Navigation : public QWidget
{
    Q_OBJECT

public:
    explicit Navigation(QWidget *parent = nullptr);
    ~Navigation();
signals:
    void entryArticlePage(QString key);

private:
    Ui::Navigation *ui;

    QString cfPath;
    QJsonDocument confJ;  // 配置JSON

    bool loadConf();  // 加载导航页配置文件
    void initView();  // 初始化视图
};

#endif  // HOMEPAGE_H
