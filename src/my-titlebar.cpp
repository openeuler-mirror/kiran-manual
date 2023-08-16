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

#include "my-titlebar.h"
#include <QAction>
#include <QApplication>
#include <QBuffer>
#include <QFile>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>

MyTitleBar::MyTitleBar(QWidget *parent)
        : QWidget(parent)
        , m_colorR(153)
        , m_colorG(153)
        , m_colorB(153)
        , m_isPressed(false)
        , m_buttonType(MIN_MAX_BUTTON)
        , m_windowBorderWidth(0)
        , m_isTransparent(false)
{
    // 初始化;
    initControl();
    initConnections();
    // 加载本地样式 MyTitle.css文件;
    loadStyleSheet("MyTitle");
}

MyTitleBar::~MyTitleBar()
= default;

// 初始化控件;
void MyTitleBar::initControl()
{
    this->setContentsMargins(5, 2 , 8, 0);

    m_pIcon = new QLabel;
    m_pTitleContent = new QLabel;

    m_pButtonMin = new QPushButton;
    m_pButtonRestore = new QPushButton;
    m_pButtonMax = new QPushButton;
    m_pButtonClose = new QPushButton;

    m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
    m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

    m_pTitleContent->setObjectName("TitleContent");
    m_pButtonMin->setObjectName("ButtonMin");
    m_pButtonRestore->setObjectName("ButtonRestore");
    m_pButtonMax->setObjectName("ButtonMax");
    m_pButtonClose->setObjectName("ButtonClose");

    QPixmap pixmap = QApplication::style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    QImage image = pixmap.toImage();
    QSize size(20, 20); // 设置缩放后的大小
    QPixmap scaledPixmap = QPixmap::fromImage(image.scaled(size));
//    ui->pushButton->setStyleSheet(QString("border-image: url(%1)").arg(scaledPixmap.toImage()));



    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    scaledPixmap.save(&buffer, "PNG");
//    ui->pushButton->setStyleSheet(QString("border-image: url(data:image/png;base64,%1)").arg(QString(byteArray.toBase64())));

    m_pButtonMin->setStyleSheet(QString("border-image: url(%1);").arg(":/resources/test/subtraction.png"));
    m_pButtonMax->setStyleSheet(QString("border-image: url(%1);").arg(":/resources/test/addition.svg"));
    m_pButtonClose->setStyleSheet(QString("border-image: url(%1);").arg(":/resources/test/close.png"));
//    m_pButtonClose->setStyleSheet(QString("border-image: url(%1);").arg(QString(byteArray.toBase64())));
    m_pButtonRestore->setStyleSheet(QString("border-image: url(%1);").arg(":/resources/test/add.png"));

    m_pButtonMin->setToolTip(QStringLiteral("最小化"));
    m_pButtonRestore->setToolTip(QStringLiteral("还原"));
    m_pButtonMax->setToolTip(QStringLiteral("最大化"));
    m_pButtonClose->setToolTip(QStringLiteral("关闭"));

    QHBoxLayout* myHLayout = new QHBoxLayout(this);
    myHLayout->addWidget(m_pIcon);
    myHLayout->addWidget(m_pTitleContent);

    myHLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    m_pSearchEdit = new QLineEdit;
    m_pSearchEdit->setStyleSheet("background: #000000; color: white");
    m_pSearchEdit->setFixedWidth(this->width()/3);
    m_pSearchEdit->setFixedHeight(EDIT_WIDTH);
    m_pSearchEdit->setPlaceholderText(tr("请输入搜索关键字"));
    m_pSearchEdit->setFocusPolicy(Qt::ClickFocus);
    QAction * pActLeft = new QAction(m_pSearchEdit);
    pActLeft->setIcon(QIcon(":/resources/test/search.svg"));
    m_pSearchEdit->addAction(pActLeft,QLineEdit::LeadingPosition);
    myHLayout->addWidget(m_pSearchEdit);
    myHLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

    myHLayout->addWidget(m_pButtonMin);
    myHLayout->addItem(new QSpacerItem(6, 6, QSizePolicy::Fixed, QSizePolicy::Fixed));
    myHLayout->addWidget(m_pButtonRestore);
    myHLayout->addItem(new QSpacerItem(6, 6, QSizePolicy::Fixed, QSizePolicy::Fixed));
    myHLayout->addWidget(m_pButtonMax);
    myHLayout->addItem(new QSpacerItem(6, 6, QSizePolicy::Fixed, QSizePolicy::Fixed));
    myHLayout->addWidget(m_pButtonClose);

    // TODO: 搜索框的回车键响应
    connect(m_pSearchEdit, &QLineEdit::returnPressed, this, [=]() {

    });

    myHLayout->setContentsMargins(5, 0, 0, 0);
    myHLayout->setSpacing(0);

    m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setFixedHeight(TITLE_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

// 信号槽的绑定;
void MyTitleBar::initConnections()
{
    connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
    connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
    connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
}

// 设置标题栏背景色,在paintEvent事件中进行绘制标题栏背景色;
// 在构造函数中给了默认值，可以外部设置颜色值改变标题栏背景色;
void MyTitleBar::setBackgroundColor(int r, int g, int b, bool isTransparent)
{
    m_colorR = r;
    m_colorG = g;
    m_colorB = b;
    m_isTransparent = isTransparent;
    // 重新绘制（调用paintEvent事件）;
    update();
}

// 设置标题栏图标;
void MyTitleBar::setTitleIcon(const QString& filePath, QSize IconSize)
{
    QPixmap titleIcon(filePath);
    m_pIcon->setPixmap(titleIcon.scaled(IconSize));
}

// 设置标题内容;
void MyTitleBar::setTitleContent(const QString& titleContent, int titleFontSize)
{
    // 设置标题字体大小;
    QFont font = m_pTitleContent->font();
    font.setPointSize(titleFontSize);
    m_pTitleContent->setFont(font);
    // 设置标题内容;
    m_pTitleContent->setText(titleContent);
    m_titleContent = titleContent;
}

// 设置标题栏长度;
void MyTitleBar::setTitleWidth(int width)
{
    this->setFixedWidth(width);
}

// 设置标题栏上按钮类型;
// 由于不同窗口标题栏上的按钮都不一样，所以可以自定义标题栏中的按钮;
// 这里提供了四个按钮，分别为最小化、还原、最大化、关闭按钮，如果需要其他按钮可自行添加设置;
void MyTitleBar::setButtonType(ButtonType buttonType)
{
    m_buttonType = buttonType;

    switch (buttonType)
    {
        case MIN_BUTTON:
        {
            m_pButtonRestore->setVisible(false);
            m_pButtonMax->setVisible(false);
        }
            break;
        case MIN_MAX_BUTTON:
        {
            m_pButtonRestore->setVisible(false);
        }
            break;
        case ONLY_CLOSE_BUTTON:
        {
            m_pButtonMin->setVisible(false);
            m_pButtonRestore->setVisible(false);
            m_pButtonMax->setVisible(false);
        }
            break;
        default:
            break;
    }
}

// 设置窗口边框宽度;
void MyTitleBar::setWindowBorderWidth(int borderWidth)
{
    m_windowBorderWidth = borderWidth;
}

// 保存窗口最大化前窗口的位置以及大小;
void MyTitleBar::saveRestoreInfo(const QPoint point, const QSize size)
{
    m_restorePos = point;
    m_restoreSize = size;
}

// 获取窗口最大化前窗口的位置以及大小;
void MyTitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
    point = m_restorePos;
    size = m_restoreSize;
}

// 绘制标题栏背景色;
void MyTitleBar::paintEvent(QPaintEvent *event)
{
    // 是否设置标题透明;
    if (!m_isTransparent)
    {
        //设置背景色;
        QPainter painter(this);
        QPainterPath pathBack;
        pathBack.setFillRule(Qt::WindingFill);
        pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.fillPath(pathBack, QBrush(QColor(m_colorR, m_colorG, m_colorB)));
    }

    // 当窗口最大化或者还原后，窗口长度变了，标题栏的长度应当一起改变;
    // 这里减去m_windowBorderWidth ，是因为窗口可能设置了不同宽度的边框;
    // 如果窗口有边框则需要设置m_windowBorderWidth的值，否则m_windowBorderWidth默认为0;
    if (this->width() != (this->parentWidget()->width() - m_windowBorderWidth))
    {
        this->setFixedWidth(this->parentWidget()->width() - m_windowBorderWidth);
    }
    QWidget::paintEvent(event);
}

// 双击响应事件，主要是实现双击标题栏进行最大化和最小化操作;
void MyTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    // 只有存在最大化、还原按钮时双击才有效;
    if (m_buttonType == MIN_MAX_BUTTON)
    {
        // 通过最大化按钮的状态判断当前窗口是处于最大化还是原始大小状态;
        // 或者通过单独设置变量来表示当前窗口状态;
        if (m_pButtonMax->isVisible())
        {
            onButtonMaxClicked();
        }
        else
        {
            onButtonRestoreClicked();
        }
    }

    return QWidget::mouseDoubleClickEvent(event);
}

// 以下通过mousePressEvent、mouseMoveEvent、mouseReleaseEvent三个事件实现了鼠标拖动标题栏移动窗口的效果;
void MyTitleBar::mousePressEvent(QMouseEvent *event)
{
    if (m_buttonType == MIN_MAX_BUTTON)
    {
        // 在窗口最大化时禁止拖动窗口;
        if (m_pButtonMax->isVisible())
        {
            m_isPressed = true;
            m_startMovePos = event->globalPos();
        }
    }
    else
    {
        m_isPressed = true;
        m_startMovePos = event->globalPos();
    }

    return QWidget::mousePressEvent(event);
}

void MyTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed)
    {
        QPoint movePoint = event->globalPos() - m_startMovePos;
        QPoint widgetPos = this->parentWidget()->pos();
        m_startMovePos = event->globalPos();
        this->parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}

void MyTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}

// 加载本地样式文件;
// 可以将样式直接写在文件中，程序运行时直接加载进来;
void MyTitleBar::loadStyleSheet(const QString &sheetName)
{
    QFile file(":/resources/" + sheetName + ".css");
    file.open(QFile::ReadOnly);
    if (file.isOpen())
    {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }
}

// 以下为按钮操作响应的槽;
void MyTitleBar::onButtonMinClicked()
{
    emit signalButtonMinClicked();
}

void MyTitleBar::onButtonRestoreClicked()
{
    m_pButtonRestore->setVisible(false);
    m_pButtonMax->setVisible(true);
    emit signalButtonRestoreClicked();
}

void MyTitleBar::onButtonMaxClicked()
{
    m_pButtonMax->setVisible(false);
    m_pButtonRestore->setVisible(true);
    emit signalButtonMaxClicked();
}

void MyTitleBar::onButtonCloseClicked()
{
    emit signalButtonCloseClicked();
}

void MyTitleBar::resizeEvent(QResizeEvent *event) {
    int width = event->size().width();
    int lineEditWidth = width / 3;
    m_pSearchEdit->setFixedWidth(lineEditWidth);
}

