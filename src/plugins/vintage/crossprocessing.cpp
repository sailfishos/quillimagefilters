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

#include "quillimagefilter.h"
#include "crossprocessing.h"
#include "vignette.h"

CrossProcessing::CrossProcessing()
{
}

CrossProcessing::~CrossProcessing()
{
}

QuillImage CrossProcessing::apply(const QuillImage &image) const
{
    QuillImage newImage(image);

    QRgb *endp = (QRgb*)(newImage.bits()+newImage.numBytes());

    // Apply the actual operation

    for (QRgb *p=(QRgb*)newImage.bits(); p<endp; p++)
    {
        QRgb rgb = *p;

        int colorvalue = 127;

        int newRed = colorvalue + (qRed(rgb) - colorvalue) * 7 / 4;
        int newGreen = colorvalue + (qGreen(rgb) - colorvalue) * 5 / 4;
        int newBlue = colorvalue + (qBlue(rgb) - colorvalue) / 4;

        if (newRed < 0) newRed = 0;
        if (newRed > 255) newRed = 255;

        if (newGreen < 0) newGreen = 0;
        if (newGreen > 255) newGreen = 255;

        if (newBlue < 0) newBlue = 0;
        if (newBlue > 255) newBlue = 255;

	*p = qRgba(newRed, newGreen, newBlue,
		   qAlpha(rgb));
    }

    newImage = Vignette::apply(newImage, .7, .5);

    return newImage;
}
