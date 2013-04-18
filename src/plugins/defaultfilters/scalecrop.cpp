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

#include <QRect>
#include <QSize>
#include "scalecrop.h"

class ScaleCropPrivate
{
public:
    QRect cropRectangle;
    QSize sizeAfter;
};

ScaleCrop::ScaleCrop()
{
    priv = new ScaleCropPrivate;
    priv->sizeAfter = QSize();
    priv->cropRectangle = QRect();

    //m_role = QuillImageFilter::Role_PreviewScale;
}

ScaleCrop::~ScaleCrop()
{
    delete priv;
}

QSize ScaleCrop::newFullImageSize(const QSize &fullImageSize) const
{
    // This preview level operation does not change full image size.
    return fullImageSize;
}

QRect ScaleCrop::newArea(const QSize &fullImageSize, const QRect &area) const
{
    // The affected area can be changed freely from the CropRectangle parameter.
    if (priv->cropRectangle.isValid())
        return priv->cropRectangle.
            intersected(QRect(QPoint(0, 0), fullImageSize));
    else
        return area;
}

int ScaleCrop::transform(int x, int numerator, int denominator)
{
    return (x * numerator + denominator - 1) / denominator;
}

QuillImage ScaleCrop::apply(const QuillImage &image) const
{
    // The input image must represent the full image

    if (image.isNull() || image.isFragment() || image.fullImageSize().isEmpty())
        return image;

    // The crop rectangle relative to the current image zoom level

    QuillImage result = image;

    if (priv->cropRectangle.isValid()) {
        QRect relativeRect = QRect(transform(priv->cropRectangle.left(),
                                             image.width(),
                                             image.fullImageSize().width()),
                                   transform(priv->cropRectangle.top(),
                                             image.height(),
                                             image.fullImageSize().height()),
                                   transform(priv->cropRectangle.width(),
                                             image.width(),
                                             image.fullImageSize().width()),
                                   transform(priv->cropRectangle.height(),
                                             image.height(),
                                             image.fullImageSize().height()));
        result = result.copy(relativeRect);
    }

    if (priv->sizeAfter.isValid())
        result = result.scaled(priv->sizeAfter);

    result.setFullImageSize(image.fullImageSize());
    result.setArea(newArea(image.fullImageSize(), image.area()));

    return result;
}

bool ScaleCrop::setOption(const QString &option, const QVariant &value)
{
    bool bOK = true;

    if (option == QuillImageFilter::CropRectangle)
        priv->cropRectangle = value.toRect();
    else if (option == QuillImageFilter::SizeAfter)
        priv->sizeAfter = value.toSize();
    else
        bOK = false;

    return bOK;
}

QVariant ScaleCrop::option(const QString &option) const
{
    if (option == QuillImageFilter::CropRectangle)
        return QVariant(priv->cropRectangle);
    else if (option == QuillImageFilter::SizeAfter)
        return QVariant(priv->sizeAfter);
    else
        return QVariant();
}

const QStringList ScaleCrop::supportedOptions() const
{
    return QStringList() << QuillImageFilter::CropRectangle
                         << QuillImageFilter::SizeAfter;
}

QuillImageFilter::QuillFilterRole ScaleCrop::role() const
{
    return QuillImageFilter::Role_PreviewScale;
}

