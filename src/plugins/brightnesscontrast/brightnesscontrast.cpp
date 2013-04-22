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
#include "brightnesscontrast.h"

class BrightnessContrastPrivate
{
public:
    float brightness, contrast;
};

BrightnessContrast::BrightnessContrast()
{
    priv = new BrightnessContrastPrivate;
    priv->brightness = 0;
    priv->contrast = 0;
}

BrightnessContrast::~BrightnessContrast()
{
    delete priv;
}

void BrightnessContrast::generateLookup(int *lookup) const
{
    // Generate lookup table - note identical behavior for each color component
    for (int i=0; i<256; i++)
    {
        lookup[i] = (int)
            ((i + priv->brightness * 2.55 - 127.5) *
             (100 + priv->contrast) / 100 + 127.5 + .5);

        if (lookup[i] > 255)
            lookup[i] = 255;
        if (lookup[i] < 0)
            lookup[i] = 0;
    }
}

QuillImage BrightnessContrast::apply(const QuillImage &image) const
{
    QuillImage result(image);

    QRgb *endp = (QRgb*)(result.bits()+result.byteCount());

    int lookup[256];

    generateLookup(lookup);

    // Apply the actual operation
    for (QRgb *p=(QRgb*)result.bits(); p<endp; p++)
    {
        QRgb rgb = *p;

	*p = qRgba(lookup[qRed(rgb)],
		   lookup[qGreen(rgb)],
		   lookup[qBlue(rgb)],
		   qAlpha(rgb));
    }
    return result;
}

bool BrightnessContrast::setOption(const QString &option, const QVariant &value)
{
    bool bOK = false;

    if (option == QuillImageFilter::Brightness) {
        float brightness = value.toDouble(&bOK);
        if (bOK)
            priv->brightness = brightness;
    }
    else if (option == QuillImageFilter::Contrast) {
        float contrast = value.toDouble(&bOK);
        if (bOK)
            priv->contrast = contrast;
    }
    else
        bOK = false;

    return bOK;
}

QVariant BrightnessContrast::option(const QString &option) const
{
    if (option == QuillImageFilter::Brightness)
        return QVariant(priv->brightness);
    else if (option == QuillImageFilter::Contrast)
        return QVariant(priv->contrast);
    else
        return QVariant();
}

const QStringList BrightnessContrast::supportedOptions() const
{
    return QStringList() << QuillImageFilter::Brightness
                         << QuillImageFilter::Contrast;
}

