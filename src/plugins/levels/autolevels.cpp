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

#include "autolevels.h"
#include "quillimagefilterfactory.h"

/*!
  The levels contrast stretching algorithm works this way:
  * Build histogram
  * Set new low so that .5% of samples are under it
  * Set new high to that 99.5% of samples are under it
  *
  * Since this is quite slow, it should operate on preview size only!
  */

QuillImageFilter *AutoLevels::generate(const QuillImage &image) const
{
    /* No image given, no reasonable filter can be constructed */
    if (image == QImage())
        return 0;

    /* Iterate through all pixels of the image */

    QRgb *endp = (QRgb*)(image.bits()+image.byteCount());

    int redSamples[256],
        greenSamples[256],
        blueSamples[256];
    int total = 0;

    for (int i=0; i<256; i++)
    {
        redSamples[i] = 0;
        greenSamples[i] = 0;
        blueSamples[i] = 0;
    }

    for (QRgb *p = (QRgb*) image.bits(); p<endp; p++)
    {
        QRgb rgb = *p;

        redSamples[qRed(rgb)]++;
        greenSamples[qGreen(rgb)]++;
        blueSamples[qBlue(rgb)]++;

        total++;
    }

    int cutoffLow = total / 200;
    int cutoffHigh = total - cutoffLow;

    /* construct filter */

    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.levels");

    if (!filter)
        return 0;

    filter->setOption(QuillImageFilter::Red,
                      QVariant(level(redSamples, cutoffLow)));
    filter->setOption(QuillImageFilter::RedMax,
                      QVariant(level(redSamples, cutoffHigh)));
    filter->setOption(QuillImageFilter::Green,
                      QVariant(level(greenSamples, cutoffLow)));
    filter->setOption(QuillImageFilter::GreenMax,
                      QVariant(level(greenSamples, cutoffHigh)));
    filter->setOption(QuillImageFilter::Blue,
                      QVariant(level(blueSamples, cutoffLow)));
    filter->setOption(QuillImageFilter::BlueMax,
                      QVariant(level(blueSamples, cutoffHigh)));

    return filter;
}

int AutoLevels::level(int* sample, int target)
{
    int i, cumulative = 0;
    for (i=0; i<256; i++)
    {
        cumulative += sample[i];
        if ((cumulative >= target) && (cumulative > 0))
            break;
    }

    return i;
}
