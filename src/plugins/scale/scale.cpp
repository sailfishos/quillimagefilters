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
#include "scale.h"



class ScalePrivate
{
public:
    QSize sizeAfter;
};

Scale::Scale()
{
    priv = new ScalePrivate;
    priv->sizeAfter = QSize();
}

Scale::~Scale()
{
    delete priv;
}

QuillImage Scale::apply(const QuillImage &image) const
{
    if (priv->sizeAfter == QSize())
        return QuillImage();

    // "Scaling" preview images - will only change the full image size
    // Because we add 1 or -1 to remove artifacts, sometimes image size is not exactlly equal to image area size.
    // Therefore, we make sure the "preview image" is not tiled image
    if (image.size() != image.area().size()&&(!image.isFragment()))
    {
        QuillImage result = image;
        result.setFullImageSize(priv->sizeAfter);
        result.setArea(QRect(QPoint(0, 0), priv->sizeAfter));
        return result;
    }

    QRect area = newArea(image.fullImageSize(), image.area());
    QuillImage result = image.scaled(area.size(),
                                     Qt::IgnoreAspectRatio,
                                     Qt::SmoothTransformation);
    result.setFullImageSize(priv->sizeAfter);
    result.setArea(area);

    return result;
}

QSize Scale::newFullImageSize(const QSize &fullImageSize) const
{
    Q_UNUSED(fullImageSize);
    return priv->sizeAfter;
}

QRect Scale::newArea(const QSize &fullImageSize, const QRect &area) const
{
    QRect result = QRect(
        QPoint(area.left() * priv->sizeAfter.width() / fullImageSize.width(),
               area.top() * priv->sizeAfter.height() / fullImageSize.height()),
        QPoint((area.left() + area.width()) * priv->sizeAfter.width() / fullImageSize.width() - 1,
               (area.top() + area.height()) * priv->sizeAfter.height() / fullImageSize.height() - 1));
    if ((result.width() > 0) && (result.height() > 0))
        return result;
    else
        return QRect();
}

bool Scale::setOption(const QString &option, const QVariant &value)
{
    bool bOK = false;

    if (option == QuillImageFilter::SizeAfter) {
        priv->sizeAfter = value.toSize();
        if (priv->sizeAfter != QSize())
            bOK = true;
    }

    return bOK;
}

QVariant Scale::option(const QString &option) const
{
    if (option == QuillImageFilter::SizeAfter)
        return QVariant(priv->sizeAfter);
    else
        return QVariant();
}

const QStringList Scale::supportedOptions() const
{
    return QStringList() << QuillImageFilter::SizeAfter;
}
