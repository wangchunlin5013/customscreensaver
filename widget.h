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
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWindow>
#include <QPixmap>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QMediaPlaylist>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void setGeometry(const QRect &rect);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setGeometry(int, int, int, int) = delete;

private:
    QPixmap *m_pixmap = nullptr;
    QMediaPlayer *m_player = nullptr;
    QMediaPlaylist *m_playerList = nullptr;
    QVideoWidget *m_videoWidget = nullptr;
};

#endif // WIDGET_H
