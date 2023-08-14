#include "home-page.h"
#include "ui_homepage.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QDebug>

HomePage::HomePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);

    // 从配置文件加载配置
    QString filepath = "/home/skyzcyou/Documents/km-config.json";
    loadConf(filepath);
    // 初始化视图
    initView();
}

HomePage::~HomePage()
{
    delete ui;
}

bool HomePage::loadConf(const QString &filepath)
{
    //判断路径以及是否正常打开
    if(filepath.isEmpty())
        return false;
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return false;

    //读取数据后关闭文件
    const QByteArray raw_data=file.readAll();
    file.close();

    //解析为Json文档
    QJsonParseError json_error;
    QJsonDocument json_doc=QJsonDocument::fromJson(raw_data,&json_error);

    qDebug() << json_doc << endl;

    //是否正常解析Json数据
    if(json_doc.isNull()||json_doc.isEmpty()||json_error.error!=QJsonParseError::NoError){
        return false;
    }
    return false;
}

void HomePage::initView()
{
    // 根据 conf 创建主页
    QVBoxLayout *homeLayout = new QVBoxLayout(this);
    homeLayout->addWidget(new QLabel("Welcome to QT Program!"));
    QPushButton *article1Btn = new QPushButton("Article 1");
    QPushButton *article2Btn = new QPushButton("Article 2");
    QPushButton *article3Btn = new QPushButton("Article 3");
    homeLayout->addWidget(article1Btn);
    homeLayout->addWidget(article2Btn);
    homeLayout->addWidget(article3Btn);


}
