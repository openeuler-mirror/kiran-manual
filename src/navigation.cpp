#include "home-page.h"
#include "ui_home-page.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::HomePage)
{
    ui->setupUi(this);

    this->cfPath = "/home/skyzcyou/Documents/km-config.json";
    // 从配置文件加载配置
    loadConf();
    // 初始化视图
    initView();
}

HomePage::~HomePage()
{
    delete ui;
}

bool HomePage::loadConf()
{
    //判断路径以及是否正常打开
    if (cfPath.isEmpty())
    {
        return false;
    }
    QFile file(cfPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    //读取数据后关闭文件
    const QByteArray rawData = file.readAll();
    file.close();

    //解析为Json文档
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(rawData, &jsonError);
    //是否正常解析Json数据
    if (jsonDoc.isNull() || jsonDoc.isEmpty() || jsonError.error != QJsonParseError::NoError)
    {
        return false;
    }
    confJ = jsonDoc;
    return true;
}

void HomePage::initView()
{
    // TODO: 根据 conf 创建主页
    QVBoxLayout *homeLayout = new QVBoxLayout(this);

    QJsonObject obj = confJ.object();
    QJsonArray kmArray = obj.value("kiran-manual").toArray();
    for (auto it = kmArray.constBegin(); it != kmArray.constEnd(); ++it)
    {
        QJsonObject obj = it->toObject();
        QString itType = obj.keys()[0];
        qDebug() << itType << endl;

        // 创建分类块
        QVBoxLayout *typeLayout = new QVBoxLayout();
        typeLayout->addWidget(new QLabel(itType));
        QHBoxLayout *itemLayout = new QHBoxLayout();
        typeLayout->addLayout(itemLayout);

        QJsonArray itArray = it->toObject().value(itType).toArray();
        for (auto i = itArray.constBegin(); i != itArray.constEnd(); ++i)
        {
            QJsonObject o = i->toObject();
            QString iName = o.value("name").toString();
            // 添加条目块
            QPushButton *iBtn = new QPushButton(iName);
            iBtn->setMaximumWidth(100);
            connect(iBtn, &QPushButton::clicked, this, [=]() {
                QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
                if (clickedButton)
                {
                    qDebug() << "Button clicked: " << itType + "->" + iName;
                    //用 emit 发信号
                    emit entryArticlePage(iName);
                }
            });

            itemLayout->addWidget(iBtn);
        }

        // 添加分类块
        homeLayout->addLayout(typeLayout);
    }
}
