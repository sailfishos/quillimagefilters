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
#include "greyscale.h"
#include "vignette.h"

Greyscale::Greyscale()
{
}

Greyscale::~Greyscale()
{
}

QuillImage Greyscale::apply(const QuillImage &image) const
{
    QuillImage newImage(image);

    QRgb *endp = (QRgb*)(newImage.bits()+newImage.byteCount());

    // Apply the actual operation
    // Greyscale, contrast +80%

    for (QRgb *p=(QRgb*)newImage.bits(); p<endp; p++)
    {
        QRgb rgb = *p;

        int colorvalue = (qRed(rgb)*299 + qGreen(rgb)*587 + qBlue(rgb)*114)
            / 1000;

        colorvalue = (colorvalue - 127) * 9 / 5 + 127;

        if (colorvalue < 0) colorvalue = 0;
        if (colorvalue > 255) colorvalue = 255;

	*p = qRgba(colorvalue, colorvalue, colorvalue,
		   qAlpha(rgb));
    }

    newImage = Vignette::apply(newImage, 1, 1);

    return newImage;
}
