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

#include "window.h"
#include "kiran-log/qt5-log-i.h"

#include <QApplication>
#include <QStandardPaths>

static const char* const projectName = "kiran-manual";
int main(int argc, char *argv[])
{

    // 获取应用程序的 share 路径
    QStringList paths = QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
    QString sharePath = paths.value(0) + "/kiran-manual/";
    qDebug() << "Share path: " << sharePath;

    // 初始化日志库
    int iRet = klog_qt5_init("", "kylinsec-session", projectName, projectName);
    if (iRet != 0)
    {
        fprintf(stderr, "klog_qt5_init field,res:%d\n", iRet);
    }

    QApplication app(argc, argv);
    Window window;
    window.show();
    return QApplication::exec();
}
