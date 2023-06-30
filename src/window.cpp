#include "main-window.h"
#include "article-pages.h"
#include "home-page.h"
#include "ui_main-window.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //	初始化UI
    initView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchArticlePage(QString key)
{
    qDebug() << key << endl;
    ui->stackedWidget->setCurrentIndex(3);
}
void MainWindow::switchHomePage(QString key)
{
    qDebug() << key << endl;
    ui->stackedWidget->setCurrentIndex(2);
}

// 初始化起始页视图
void MainWindow::initView()
{
    HomePage *hp = new HomePage();
    ArticlePage *ap = new ArticlePage();
    // 将主页和文章页面添加到 QStackedWidget 中, 并设定主页
    ui->stackedWidget->addWidget(hp);
    ui->stackedWidget->addWidget(ap);
    ui->stackedWidget->setCurrentWidget(hp);

    connect(hp, &HomePage::entryArticlePage, this, &MainWindow::switchArticlePage);
    connect(ap, &ArticlePage::backHome, this, &MainWindow::switchHomePage);

}
