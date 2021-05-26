/*
 * Copyright (C) 2020 ~ 2021 Uniontech Software Technology Co., Ltd.
 *
 * Author:     wangchunlin<wangchunlin@uniontech.com>
 *
 * Maintainer: wangchunlin<wangchunlin@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "widget.h"
#include <QApplication>
#include <QtDebug>
#include <QWindow>
#include <QMessageBox>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;   // 自定义屏保的显示主界面
    if (argc >= 3 && QString(argv[1]) == QString("-window-id")) {
        WId windowHwnd = WId(QString(argv[2]).toULongLong());
        QWindow *window = QWindow::fromWinId(windowHwnd);

        w.setProperty("_q_embedded_native_parent_handle",QVariant(windowHwnd));
        w.winId();
        w.windowHandle()->setParent(window);
        QRect wRect = window->screen()->geometry();
        w.setGeometry(QRect(0, 0, wRect.width(), wRect.height()));
    }

    w.show();

    return a.exec();
}
