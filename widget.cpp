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
#include <QLabel>
#include <QVBoxLayout>
#include <QtDebug>
#include <QStandardPaths>
#include <QString>
#include <QScreen>
#include <QPainter>
#include <QPaintEvent>
#include <QDir>
#include <QMimeDatabase>
#include <QMimeType>
#include "thumbnailprovider.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    QString path = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first() + "/.config/deepin/deepin-screensaver/customscreensaver/";
    QDir dir(path);
    QDir::Filters filters = QDir::Files | QDir::NoDotAndDotDot | QDir::Readable;
    QFileInfoList infoList = dir.entryInfoList(filters);
    if (infoList.isEmpty()) {
        qWarning()<<"Error:no file!";
        return;
    }
    QFileInfo fileInfo = infoList.first();
    static bool created = false;
    if (!created) {
        created = true;
        bool result = ThumbnailProvider::createThumbnail(fileInfo);
        qDebug()<<"==========create thumbnail:"<<result;
    }

    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(fileInfo);
    qDebug()<<"mime.name():"<<mime.name()<<fileInfo.absoluteFilePath();
    if (mime.name().startsWith("image/")) {
        m_pixmap = new QPixmap(fileInfo.absoluteFilePath());
    } else if (mime.name().startsWith("video/")) {
        m_playerList = new QMediaPlaylist;
        m_playerList->addMedia(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
        m_playerList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

        m_player = new QMediaPlayer;
        m_player->setPlaylist(m_playerList);

//        connect(m_player, &QMediaPlayer::positionChanged, this, [=](qint64 pos){
//            qDebug()<<"==pos:"<<pos;
//        });

        m_videoWidget = new QVideoWidget(this);
        m_player->setVideoOutput(m_videoWidget);

        m_videoWidget->setGeometry(this->geometry());
        m_videoWidget->show();
        m_player->play();

    } else {
        qWarning()<<"Error:unknow format:"<<mime.name();
        return;
    }

    setWindowFlag(Qt::WindowTransparentForInput, true);
}

Widget::~Widget()
{

}

void Widget::setGeometry(const QRect &rect)
{
    if (m_player && m_videoWidget) {
        m_videoWidget->setGeometry(rect);
    }
    return QWidget::setGeometry(rect);
}

void Widget::paintEvent(QPaintEvent *event)
{
    if (m_pixmap) {
        auto pix = m_pixmap->scaled(this->windowHandle()->screen()->geometry().size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        auto trueSize = this->windowHandle()->screen()->geometry().size();

        if (pix.width() > trueSize.width() || pix.height() > trueSize.height()) {
            pix = pix.copy(QRect(static_cast<int>((pix.width() - trueSize.width()) / 2.0),
                                 static_cast<int>((pix.height() - trueSize.height()) / 2.0),
                                 trueSize.width(),
                                 trueSize.height()));
        }

        qreal scale = devicePixelRatioF();

        QPainter pa(this);
        pa.drawPixmap(event->rect().topLeft(), pix, QRectF(QPointF(event->rect().topLeft()) * scale, QSizeF(event->rect().size()) * scale));
    }

    return QWidget::paintEvent(event);
}

