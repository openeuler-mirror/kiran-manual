#include "main-window.h"
#include "ui_mainwindow.h"
#include "home-page.h"
#include "article-page.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //	初始化UI
    initView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 初始化起始页视图
void MainWindow::initView()
{
    HomePage *hp = new HomePage();
    ArticlePage *ap = new ArticlePage();
    // 将主页和文章页面添加到 QStackedWidget 中, 并设定主页
    ui->stackedWidget->addWidget(hp);
    ui->stackedWidget->addWidget(ap);
    ui->stackedWidget->setCurrentWidget(ap);

}
