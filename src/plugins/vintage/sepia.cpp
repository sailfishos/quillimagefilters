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
#include "sepia.h"
#include "vignette.h"

Sepia::Sepia()
{
}

Sepia::~Sepia()
{
}

QuillImage Sepia::apply(const QuillImage &image) const
{
    QuillImage newImage(image);

    QRgb *endp = (QRgb*)(newImage.bits()+newImage.byteCount());

    // Apply the actual operation

    for (QRgb *p=(QRgb*)newImage.bits(); p<endp; p++)
    {
        QRgb rgb = *p;

        int colorvalue = (qRed(rgb)*299 + qGreen(rgb)*587 + qBlue(rgb)*114)
            / 1000;

        int newRed, newGreen, newBlue;

        if (colorvalue < 128) {
            newRed = colorvalue;
            newGreen = colorvalue * 104 / 128;
            newBlue = colorvalue * 69 / 128;
        }
        else {
            newRed = colorvalue;
            newGreen = 104 + (colorvalue-128) * 152 / 128;
            newBlue = 69 + (colorvalue-128) * 187 / 128;
        }

        if (newRed < 0) newRed = 0;
        if (newRed > 255) newRed = 255;

        if (newGreen < 0) newGreen = 0;
        if (newGreen > 255) newGreen = 255;

        if (newBlue < 0) newBlue = 0;
        if (newBlue > 255) newBlue = 255;

	*p = qRgba(newRed, newGreen, newBlue,
		   qAlpha(rgb));
    }

    newImage = Vignette::apply(newImage, 1.0, 0.5);

    return newImage;
}
