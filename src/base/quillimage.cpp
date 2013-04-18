/****************************************************************************
**
** Copyright (C) 2009-11 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Pekka Marjola <pekka.marjola@nokia.com>
**
** This file is part of the Quill Image Filters package.
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/

#include <QSize>
#include <QRect>

#include "quillimage.h"

class QuillImagePrivate
{
public:
    QSize fullImageSize;
    QSize targetSize;
    QRect area;
    int z;
};

QuillImage::QuillImage():QImage()
{
    priv = new QuillImagePrivate;
    priv->fullImageSize = QSize();
    priv->targetSize = QSize();
    priv->area = QRect();
    priv->z = 0;
}

QuillImage::QuillImage(const QImage &image) : QImage(image)
{
    priv = new QuillImagePrivate;
    priv->fullImageSize = image.size();
    priv->targetSize = image.size();
    priv->area = QRect(QPoint(0, 0), image.size());
    priv->z = 0;
}

QuillImage::QuillImage(const QImage &image, const QSize &fullImageSize) : QImage(image)
{
    priv = new QuillImagePrivate;
    priv->fullImageSize = fullImageSize;
    priv->targetSize = image.size();
    priv->area = QRect(QPoint(0, 0), fullImageSize);
    priv->z = 0;
}

QuillImage::QuillImage(const QuillImage &image) : QImage(image)
{
    priv = new QuillImagePrivate;
    priv->fullImageSize = image.fullImageSize();
    priv->targetSize = image.targetSize();
    priv->area = image.area();
    priv->z = image.z();
}

QuillImage::QuillImage(const QuillImage &quillImage, const QImage &image) : QImage(image)
{
    priv = new QuillImagePrivate;
    priv->fullImageSize = quillImage.fullImageSize();
    priv->targetSize = quillImage.targetSize();
    priv->area = quillImage.area();
    priv->z = quillImage.z();
}

QuillImage::~QuillImage()
{
    delete priv;
}

QSize QuillImage::fullImageSize() const
{
    return priv->fullImageSize;
}

QSize QuillImage::targetSize() const
{
    return priv->targetSize;
}

bool QuillImage::isFragment() const
{
    return (priv->area != QRect(QPoint(0, 0), priv->fullImageSize));
}

QRect QuillImage::area() const
{
    return priv->area;
}

int QuillImage::z() const
{
    return priv->z;
}

void QuillImage::setFullImageSize(const QSize &size)
{
    priv->fullImageSize = size;
}

void QuillImage::setTargetSize(const QSize &size)
{
    priv->targetSize = size;
}

void QuillImage::setArea(const QRect &area)
{
    priv->area = area;
}

void QuillImage::setZ(int z)
{
    priv->z = z;
}

QuillImage &QuillImage::operator=(const QuillImage &other)
{
    QImage::operator=(other);
    priv->fullImageSize = other.fullImageSize();
    priv->targetSize = other.targetSize();
    priv->area = other.area();
    priv->z = other.z();

    return *this;
}

