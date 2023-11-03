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

#include <QApplication>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QStandardPaths>
#include <QTranslator>
#include "constants.h"
#include "kiran-log/qt5-log-i.h"
#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    int iRet = klog_qt5_init("", "kylinsec-session", PROJECT_NAME, PROJECT_NAME);
    if (iRet != 0)
    {
        fprintf(stderr, "klog_qt5_init field,res:%d\n", iRet);
    }
    KLOG_INFO() << PROJECT_NAME << "Start ^_^";

    QTranslator translator;
    if (!translator.load(QLocale(), PROJECT_NAME, ".", TRANSLATE_PREFIX, ".qm"))
    {
        KLOG_WARNING("Load translator failed for %s.", PROJECT_NAME);
    }
    else
    {
        QCoreApplication::installTranslator(&translator);
    }

    Kiran::Window window;
    window.resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    int screeNum = QApplication::desktop()->screenNumber(QCursor::pos());
    QRect screenGeometry = QApplication::desktop()->screenGeometry(screeNum);
    window.move(screenGeometry.x() + (screenGeometry.width() - window.width()) / 2,
                screenGeometry.y() + (screenGeometry.height() - window.height()) / 2);
    window.show();

    return QApplication::exec();
}
