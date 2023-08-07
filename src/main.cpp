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
#include "constants.h"
#include "kiran-log/qt5-log-i.h"
#include <QApplication>
#include <QStandardPaths>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QTranslator>

void initKiranLog();

int main(int argc, char *argv[])
{
    initKiranLog();
    QApplication app(argc, argv);

    QTranslator trans;
    if (trans.load(DATA_FOLDER "kiran-manual." + QLocale().name() + ".qm")) {
        QCoreApplication::installTranslator(&trans);
    }

    Window window;
    window.resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    // 居中显示
    QRect availableGeometry = QApplication::desktop()->availableGeometry();
    int x = (availableGeometry.width() - window.width()) / 2;
    int y = (availableGeometry.height() - window.height()) / 2;
    window.move(x, y);
    window.show();
    return QApplication::exec();
}

// 初始化日志库
void initKiranLog(){
    int iRet = klog_qt5_init("", "kylinsec-session", PROJECT_NAME, PROJECT_NAME);
    if (iRet != 0)
    {
        fprintf(stderr, "klog_qt5_init field,res:%d\n", iRet);
    }
    KLOG_INFO() << PROJECT_NAME << "Start ^_^";
}