#include "window.h"
#include "article.h"
#include "navigation.h"
#include "ui_window.h"

#include <QDebug>

Window::Window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Window)
{
    ui->setupUi(this);
    //	初始化UI
    initView();
}

Window::~Window()
{
    delete ui;
}

void Window::switchArticlePage(QString key)
{
    qDebug() << key << endl;
    ui->stackedWidget->setCurrentIndex(3);
}
void Window::switchHomePage(QString key)
{
    qDebug() << key << endl;
    ui->stackedWidget->setCurrentIndex(2);
}

// 初始化起始页视图
void Window::initView()
{
    Navigation *hp = new Navigation();
    Article *ap = new Article();
    // 将主页和文章页面添加到 QStackedWidget 中, 并设定主页
    ui->stackedWidget->addWidget(hp);
    ui->stackedWidget->addWidget(ap);
    ui->stackedWidget->setCurrentWidget(hp);

    connect(hp, &Navigation::entryArticlePage, this, &Window::switchArticlePage);
    connect(ap, &Article::backHome, this, &Window::switchHomePage);

}
