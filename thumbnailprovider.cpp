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
#include "thumbnailprovider.h"
#include <QMimeDatabase>
#include <QMimeType>
#include <QDebug>
#include <QImage>
#include <QImageReader>
#include <QDir>
#include <QStandardPaths>
#include <libffmpegthumbnailer/videothumbnailer.h>

ThumbnailProvider::ThumbnailProvider()
{

}

bool ThumbnailProvider::createThumbnail(const QFileInfo &info)
{
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(info);
    QString mimeTypeName = mime.name();
    QString filePath = info.absoluteFilePath();

    QString imagePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first() + "/.config/deepin/deepin-screensaver/cover/";

    if (mimeTypeName.startsWith("video/")) {
        ffmpegthumbnailer::VideoThumbnailer vt(1920, false, true, 20, false);

        QString savePath = imagePath + "deepin-autonomic-screensaver_temp.png";
        std::string path(filePath.toUtf8().constData());
        std::string outputFile(savePath.toUtf8().constData());
        vt.generateThumbnail(path, ThumbnailerImageTypeEnum::Png, outputFile);
        filePath = savePath;
    }

    QScopedPointer<QImage> image(new QImage());

    // 读取图片文件生成缩略图
    if (mimeTypeName.startsWith("image/") || mimeTypeName.startsWith("video/")) {

        QImageReader reader(filePath);
        if (!reader.canRead()) {
            QString errorString = reader.errorString();
            qDebug()<<"Error:image can not read,and:"<<errorString;
            return false;
        }
        reader.setScaledSize(reader.size().scaled(m_imageWidth, m_imageHeight, Qt::IgnoreAspectRatio));
        reader.setAutoTransform(true);

        if (!reader.read(image.data())) {
            QString errorString = reader.errorString();
            qDebug()<<"Error:read image error,and:"<<errorString;
            return false;
        }

        image->operator =(image->scaled(m_imageWidth, m_imageHeight, Qt::IgnoreAspectRatio));
        QString savePath = imagePath + "deepin-autonomic-screensaver@3x.png";
        if (!image->save(savePath, Q_NULLPTR, 80)) {
            QString errorString = QStringLiteral("Can not save image to ") + savePath;
            qDebug()<<errorString;
            return false;
        }
        savePath = imagePath + "deepin-autonomic-screensaver@2x.png";
        image->scaledToHeight(200, Qt::SmoothTransformation).save(savePath);
        savePath = imagePath + "deepin-autonomic-screensaver.png";
        image->scaledToHeight(100, Qt::SmoothTransformation).save(savePath);

        if (mimeTypeName.startsWith("video/")) {
            // 删除临时文件
            QDir dir;
            dir.remove(filePath);
        }
    }

    return true;
}
