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

ThumbnailProvider::ThumbnailProvider()
{

}

bool ThumbnailProvider::createThumbnail(const QFileInfo &info)
{
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(info);
    QString mimeTypeName = mime.name();
    QString filePath = info.absoluteFilePath();
    QString imagePath = "/usr/lib/deepin-screensaver/modules/cover/";
    QScopedPointer<QImage> image(new QImage());

    // 图片文件缩略图生成示例代码
    if (mimeTypeName.startsWith("image/")) {
        QString suffix = mimeTypeName.replace("image/", "");

        QImageReader reader(filePath, suffix.toLatin1());
        if (!reader.canRead()) {
            QString errorString = reader.errorString();
            qDebug()<<"Error:image can not read,and:"<<errorString;
            return false;
        }
        const QSize &imageSize = reader.size();
        if (imageSize.width() > m_imageWidth || imageSize.height() > m_imageHeight || mime.name() == "image/svg+xml") {
            reader.setScaledSize(reader.size().scaled(m_imageWidth, m_imageHeight, Qt::KeepAspectRatio));
        }
        reader.setAutoTransform(true);

        if (!reader.read(image.data())) {
            QString errorString = reader.errorString();
            qDebug()<<"Error:read image error,and:"<<errorString;
            return false;
        }

        if (image->width() > m_imageWidth || image->height() > m_imageHeight) {
            image->operator =(image->scaled(m_imageWidth, m_imageHeight, Qt::KeepAspectRatio));
        }
        QString savePath = imagePath+"customscreensaver@3x.png";
        if (!image->save(savePath, Q_NULLPTR, 80)) {
            QString errorString = QStringLiteral("Can not save image to ") + savePath;
            return false;
        }
    }


    return true;
}
