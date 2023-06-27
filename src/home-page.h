#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();
private:
    Ui::HomePage *ui;

    QJsonDocument *conf; 					// 配置JSON

    bool loadConf(const QString &filepath); // 加载导航页配置文件
    void initView();  						// 初始化视图
};

#endif // HOMEPAGE_H
