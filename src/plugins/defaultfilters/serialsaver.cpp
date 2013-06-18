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

#include <QString>
#include <QSize>
#include <setjmp.h>

#include <QByteArray>

extern "C" {
#include <jpeglib.h>
#include <stdio.h>
}

#include "quillimage.h"
#include "serialsaver.h"


struct my_error_mgr : public jpeg_error_mgr {
    jmp_buf setjmp_buffer;
};

static void my_error_exit (j_common_ptr cinfo)
{
    my_error_mgr* myerr = (my_error_mgr*) cinfo->err;
    longjmp(myerr->setjmp_buffer, 1);
}

class SerialSaverPrivate
{
public:
    struct jpeg_compress_struct cinfo;
    struct my_error_mgr error;
    FILE *file; // Using the C file type here,
                // could this be done in more "Qt" way?
    int linesLeft;
    bool hasError;
};

SerialSaver::SerialSaver(const QString &fileName, const QSize &fullImageSize,
                         const QByteArray &exifDump)
{
    priv = new SerialSaverPrivate;
    priv->linesLeft = fullImageSize.height();
    priv->hasError = false;

    priv->cinfo.err = jpeg_std_error(&priv->error);
    priv->error.error_exit = my_error_exit;
    jpeg_create_compress(&priv->cinfo);

    priv->cinfo.image_width = fullImageSize.width();
    priv->cinfo.image_height = fullImageSize.height();
    priv->cinfo.input_components = 3;
    priv->cinfo.in_color_space = JCS_RGB;

    priv->file = fopen(fileName.toLocal8Bit().constData(), "w");

    jpeg_stdio_dest(&priv->cinfo, priv->file);

    jpeg_set_defaults(&priv->cinfo);
    jpeg_set_quality(&priv->cinfo, 85, true);
    jpeg_start_compress(&priv->cinfo, true);

    if (exifDump.size() > 0)
        jpeg_write_marker(&priv->cinfo, JPEG_APP0+1, (const JOCTET*)exifDump.constData(),
                          exifDump.size());
}

SerialSaver::~SerialSaver()
{
    if (!isFinished() && !priv->hasError)
        jpeg_destroy_compress(&priv->cinfo);
    delete priv;
}

void SerialSaver::errorExit()
{
    priv->hasError = true;
    jpeg_destroy_compress(&priv->cinfo);
    if (priv->file)
        fclose(priv->file);
    priv->file = 0;
}

bool SerialSaver::process(const QuillImage &image)
{
    if (!priv->file) {
        errorExit();
        return false;
    }

    uchar *imageData = data(image);
    uchar **rows = (uchar**) new int[image.height()];

    for (int y=0; y<image.height(); y++)
    {
        rows[y] = imageData + (image.width() * 3) * y;
    }

    if (!setjmp(priv->error.setjmp_buffer)) {
        int result = jpeg_write_scanlines(&priv->cinfo, rows, image.height());
        priv->linesLeft -= image.height();
        if (result == 0)
            errorExit();
        else if (isFinished()) {
            jpeg_finish_compress(&priv->cinfo);
            jpeg_destroy_compress(&priv->cinfo);
            fclose(priv->file);
            priv->file = 0;
        }
    } else
        errorExit();

    delete[] rows;
    delete[] imageData;

    return !priv->hasError;
}

bool SerialSaver::isFinished() const
{
    return priv->linesLeft <= 0;
}

uchar *SerialSaver::data(const QuillImage &image)
{
    uchar *data = new uchar[image.byteCount() * 3 / 4 + 1];
    uchar *d = data;
    QRgb rgbp = qRgba(0, 0, 0, 0);

    for (int y=0; y<image.height(); y++) {
        QRgb *line = (QRgb*)image.scanLine(y);
        for (int x=0; x<image.width(); x++)
        {
            QRgb rgb = line[x];
            // Very simple alpha elimination
            if (qAlpha(rgb) == 0)
                rgb = rgbp;
            *(d++) = qRed(rgb);
            *(d++) = qGreen(rgb);
            *(d++) = qBlue(rgb);
            rgbp = rgb;
        }
    }

    return data;
}

