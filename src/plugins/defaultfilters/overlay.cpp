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

#include <QImageReader>
#include <QPainter>

#include "quillimagefilter.h"
#include "overlay.h"



class OverlayPrivate
{
public:
    QImage buffer;
    QRect bufferRect;
};

Overlay::Overlay()
{
    priv = new OverlayPrivate;
    priv->buffer = QImage();
    priv->bufferRect = QRect();

    //m_role = QuillImageFilter::Role_Overlay;
}

Overlay::~Overlay()
{
    delete priv;
}

QuillImage Overlay::apply(const QuillImage &image) const
{
    QuillImage result;
    if (!priv->buffer.isNull())
        result = priv->buffer;
    else
    {
        result = QuillImage(QImage(priv->bufferRect.size(),
                                   QImage::Format_ARGB32));
        result.fill(qRgba(0, 0, 0, 0));
    }

    result.setArea(priv->bufferRect);
    result.setFullImageSize(image.fullImageSize());

    QPainter painter(&result);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);

    painter.drawImage(image.area().topLeft() - priv->bufferRect.topLeft(),
                      image);

    return result;
}

bool Overlay::setOption(const QString &option, const QVariant &value)
{
    bool bOK = true;

    if (option == QuillImageFilter::CropRectangle)
        priv->bufferRect = value.toRect();
    else if (option == QuillImageFilter::Background)
        priv->buffer = value.value<QImage>();
    else
        bOK = false;

    return bOK;
}

QVariant Overlay::option(const QString &option) const
{
    if (option == QuillImageFilter::CropRectangle)
        return QVariant(priv->bufferRect);
    else if (option == QuillImageFilter::Background)
        return QVariant(priv->buffer);
    else
        return QVariant();
}

const QStringList Overlay::supportedOptions() const
{
    return QStringList() << QuillImageFilter::CropRectangle
                         << QuillImageFilter::Background;
}


QuillImageFilter::QuillFilterRole Overlay::role() const
{
    return QuillImageFilter::Role_Overlay;
}
