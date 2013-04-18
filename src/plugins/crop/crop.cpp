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

#include <QImage>

#include "quillimagefilterinterface.h"
#include "crop.h"



class CropPrivate
{
public:
    QRect cropRectangle;
};

Crop::Crop()
{
    priv = new CropPrivate;
    priv->cropRectangle = QRect();
}

Crop::~Crop()
{
    delete priv;
}

QuillImage Crop::apply(const QuillImage &image) const
{
    // Translate and scale the crop rectangle from "big picture"
    // coordinates to ones relative to the crop area.

    QRect translatedRect =
        priv->cropRectangle.translated(-image.area().topLeft());

    float widthScale = image.width() * 1.0 / image.area().width();
    float heightScale = image.height() * 1.0 / image.area().height();

    // Note: if both translate and scale are applied (preview tiling),
    // there is a chance for rounding errors resulting in artifacts.

    QRect transformedRect = QRect(translatedRect.left() * widthScale,
                                  translatedRect.top() * heightScale,
                                  translatedRect.width() * widthScale,
                                  translatedRect.height() * heightScale);

    QRect targetRect =
        transformedRect.intersected(QRect(QPoint(0, 0), image.size()));


    QuillImage result = QuillImage(image.copy(targetRect));
    result.setFullImageSize(newFullImageSize(image.fullImageSize()));
    result.setArea(newArea(image.fullImageSize(), image.area()));

    return result;
}

QSize Crop::newFullImageSize(const QSize &fullImageSize) const
{
    return QRect(QPoint(0, 0), fullImageSize).
        intersected(priv->cropRectangle).size();
}

QRect Crop::newArea(const QSize &fullImageSize, const QRect &area) const
{
    Q_UNUSED(fullImageSize);
    return area.intersected(priv->cropRectangle).
        translated(-priv->cropRectangle.topLeft());
}

bool Crop::setOption(const QString &option, const QVariant &value)
{
    bool bOK = true;

    if (option == QuillImageFilter::CropRectangle)
        priv->cropRectangle = value.toRect();
    else
        bOK = false;

    return bOK;
}

QVariant Crop::option(const QString &option) const
{
    if (option == QuillImageFilter::CropRectangle)
        return QVariant(priv->cropRectangle);
    else
        return QVariant();
}

const QStringList Crop::supportedOptions() const
{
    return QStringList() << QuillImageFilter::CropRectangle;
}

