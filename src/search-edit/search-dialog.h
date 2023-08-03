//
// Created by skyzcyou on 2023/8/2.
//

#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
/**
 * 搜索对话框 - 提供搜索时的快捷工具
 * */
class SearchDialog : public QDialog
{
Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog() override;
    // 设置搜索的文本
    void setSearchText(const QString &text);

signals:
    // 发送上一个和下一个信号
    void prevClicked();
    void nextClicked();

private slots:
    void prevPosition();
    void nextPosition();

private:
    QPushButton *m_prevButton{};
    QPushButton *m_nextButton{};
    QLabel *m_countLabel{};
    QString m_searchText{};
    QPushButton *m_closeButton{};

private:
    void init();
};