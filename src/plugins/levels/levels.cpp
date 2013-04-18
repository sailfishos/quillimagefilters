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
#include "levels.h"




class LevelsPrivate
{
public:
    int redMin, greenMin, blueMin, redMax, greenMax, blueMax;
};

Levels::Levels()
{
    priv = new LevelsPrivate;
    priv->redMin = 0;
    priv->greenMin = 0;
    priv->blueMin = 0;
    priv->redMax = 255;
    priv->greenMax = 255;
    priv->blueMax = 255;
}

Levels::~Levels()
{
    delete priv;
}

int Levels::level(int min, int max, int i) const
{
    int n;

    if (max == min)
        n = min;
    else
        n = (i - min) * 255 / (max - min);

    if (n >= 255)
        return 255;
    if (n <= 0)
        return 0;
    return n;
}

QuillImage Levels::apply(const QuillImage &image) const
{
    QuillImage newImage(image);

    QRgb *endp = (QRgb*)(newImage.bits()+newImage.numBytes());

    int lookupRed[256], lookupGreen[256], lookupBlue[256];

    // Generate lookup tables for each color component

    for (int i=0; i<256; i++)
    {
        lookupRed[i] = level(priv->redMin, priv->redMax, i);
        lookupGreen[i] = level(priv->greenMin, priv->greenMax, i);
        lookupBlue[i] = level(priv->blueMin, priv->blueMax, i);
    }

    // Apply the actual operation

    for (QRgb *p=(QRgb*)newImage.bits(); p<endp; p++)
    {
        QRgb rgb = *p;

	*p = qRgba(lookupRed[qRed(rgb)],
		   lookupGreen[qGreen(rgb)],
		   lookupBlue[qBlue(rgb)],
		   qAlpha(rgb));
    }

    return newImage;
}

bool Levels::setIntFromVariant(int *value, const QVariant &variant) const
{
    bool bOK;
    int v = variant.toInt(&bOK);
    if (bOK)
        *value = v;
    return bOK;
}

bool Levels::setOption(const QString &option, const QVariant &value)
{
    bool bOK = false;

    if (option == QuillImageFilter::Red)
        bOK = setIntFromVariant(&priv->redMin, value);
    else if (option == QuillImageFilter::Green)
        bOK = setIntFromVariant(&priv->greenMin, value);
    else if (option == QuillImageFilter::Blue)
        bOK = setIntFromVariant(&priv->blueMin, value);
    else if (option == QuillImageFilter::RedMax)
        bOK = setIntFromVariant(&priv->redMax, value);
    else if (option == QuillImageFilter::GreenMax)
        bOK = setIntFromVariant(&priv->greenMax, value);
    else if (option == QuillImageFilter::BlueMax)
        bOK = setIntFromVariant(&priv->blueMax, value);

    return bOK;
}

QVariant Levels::option(const QString &option) const
{
    if (option == QuillImageFilter::Red)
        return QVariant(priv->redMin);
    else if (option == QuillImageFilter::Green)
        return QVariant(priv->greenMin);
    else if (option == QuillImageFilter::Blue)
        return QVariant(priv->blueMin);
    else if (option == QuillImageFilter::RedMax)
        return QVariant(priv->redMax);
    else if (option == QuillImageFilter::GreenMax)
        return QVariant(priv->greenMax);
    else if (option == QuillImageFilter::BlueMax)
        return QVariant(priv->blueMax);
    else
        return QVariant();
}

const QStringList Levels::supportedOptions() const
{
    return QList<QString>() << QuillImageFilter::Red
                            << QuillImageFilter::Green
                            << QuillImageFilter::Blue
                            << QuillImageFilter::RedMax
                            << QuillImageFilter::GreenMax
                            << QuillImageFilter::BlueMax;
}

