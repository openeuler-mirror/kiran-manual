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

#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
/**
 * 搜索对话框 - 提供搜索时的快捷工具
 * */
namespace Kiran
{
class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = nullptr);
    ~SearchDialog() override;
    // 设置搜索的文本
    void setSearchText(const QString &text);
    // 设置匹配数量
    void setMatchCount(int count, int index);

signals:
    // 发送上一个和下一个信号
    void sdPrevClicked();
    void sdNextClicked();
    void sdCloseClicked();

private:
    void init();

    QPushButton *m_prevButton{};
    QPushButton *m_nextButton{};
    QLabel *m_countLabel{};
    QString m_searchText{};
    QPushButton *m_closeButton{};
};
}  // namespace Kiran