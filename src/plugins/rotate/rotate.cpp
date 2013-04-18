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

#include "quillimagefilter.h"
#include "rotate.h"


class RotatePrivate
{
public:
    int angle;
};

Rotate::Rotate()
{
    priv = new RotatePrivate;
    priv->angle = 90;
}

Rotate::~Rotate()
{
    delete priv;
}

QuillImage Rotate::apply(const QuillImage &image) const
{
    QuillImage result = image;
    result = image.transformed(QTransform().rotate(priv->angle));

    result.setFullImageSize(newFullImageSize(image.fullImageSize()));
    result.setArea(newArea(image.fullImageSize(), image.area()));

    return result;
}

QSize Rotate::newFullImageSize(const QSize &fullImageSize) const
{
    if ((priv->angle % 180 == 90) ||
        (priv->angle % 180 == -90))
        return QSize(fullImageSize.height(), fullImageSize.width());
    else
        return fullImageSize;
}

QRect Rotate::newArea(const QSize &fullImageSize, const QRect &area) const
{
    switch (priv->angle % 360)
    {
    case 0:
        return area;
    case 180:
    case -180:
    return QRect(fullImageSize.width()-area.right() - 1,
                 fullImageSize.height()-area.bottom() - 1,
                 area.width(), area.height());
    case 90:
    case -270:
    return QRect(fullImageSize.height()-area.bottom() - 1,
                 area.left(),
                 area.height(), area.width());

    case 270:
    case -90:
    return QRect(area.top(),
                 fullImageSize.width()-area.right() - 1,
                 area.height(), area.width());
    default:
    return area;
    }
}

bool Rotate::setOption(const QString &option, const QVariant &value)
{
    bool bOK = true;

    if (option == QuillImageFilter::Angle) {
        int angle = value.toInt(&bOK);
        if (bOK)
            priv->angle = angle;
    }
    else
        bOK = false;

    return bOK;
}

QVariant Rotate::option(const QString &option) const
{
    if (option == QuillImageFilter::Angle)
        return QVariant(priv->angle);
    else
        return QVariant();
}

const QStringList Rotate::supportedOptions() const
{
    return QStringList() << QuillImageFilter::Angle;
}

