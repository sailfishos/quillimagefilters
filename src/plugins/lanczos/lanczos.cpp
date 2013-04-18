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
#include <cmath>
#include "quillimagefilter.h"
#include "lanczos.h"


Lanczos::Lanczos() : windowSize(3), kernel(0), targetSize()
{
}

Lanczos::~Lanczos()
{
    delete[] kernel;
}

bool Lanczos::setOption(const QString &option, const QVariant &value)
{
    bool bOK = true;

    if (option == QuillImageFilter::Radius) {
        int newWindowSize = value.toInt();
        if ((newWindowSize==2) || (newWindowSize==3)) {
            if ((newWindowSize != windowSize) || !kernel) {
                windowSize = newWindowSize;
                createKernel();
            }
        }
        else
            bOK=false;
    }
    else if (option == QuillImageFilter::SizeAfter)
        targetSize = value.toSize();
    else
        bOK = false;

    return bOK;
}

QVariant Lanczos::option(const QString &option) const
{
    if (option == QuillImageFilter::Radius)
        return QVariant(windowSize);
    else if (option == QuillImageFilter::SizeAfter)
        return QVariant(targetSize);
    else
        return QVariant();
}

const QStringList Lanczos::supportedOptions() const
{
    return QStringList() << QuillImageFilter::Radius
                         << QuillImageFilter::SizeAfter;
}


float Lanczos::sinc(float x)
{
    const float pi = 3.141592653589793;
    return sin(pi*x)/pi/x;
}

float Lanczos::lanczosFunction(float x)
{
    if ((x < -windowSize) || (x > windowSize))
        return 0;
    else if (x == 0)
        return 1;
    else
        return sinc(x)*sinc(x/windowSize);
}

void Lanczos::createKernel()
{
    delete[] kernel;
    kernel = new float[129];

    for (int i=0; i<129; i++)
        kernel[i] = lanczosFunction((i-64)/16.0);
}

float Lanczos::kernelValue(float x) const
{
    return kernel[(int)(x*16)+64];
}

void Lanczos::cap(float *value)
{
    if (*value < 0)
        *value = 0;
    else if (*value > 255)
        *value = 255;
}

QuillImage Lanczos::apply(const QuillImage &image) const
{
    // Invalid source or target size, no operation
    if (!targetSize.isValid() || (image.isNull()))
        return image;

    if (!kernel)
        const_cast<Lanczos*>(this)->createKernel();

    // Lanczos is only for upscaling, downscaling defaults to linear
    if ((image.width() > targetSize.width()) ||
        (image.height() > targetSize.height()))
        return QuillImage(image.scaled(targetSize,
                                       Qt::IgnoreAspectRatio,
                                       Qt::SmoothTransformation));

    QImage passImage(QSize(targetSize.width(), image.height()),
                     QImage::Format_RGB32);

    QImage targetImage(targetSize, QImage::Format_RGB32);

    float xRatio = image.width() / ((float)targetSize.width());
    float yRatio = image.height() / ((float)targetSize.height());

    for (int y=0; y<image.height(); y++)
        for (int x=0; x<targetImage.width(); x++) {
            float red = 0, green = 0, blue = 0;

            float sourceCenterX = x*xRatio;

            float sourceCenterFracX = 1 - sourceCenterX + floor(sourceCenterX);

            float sum = 0;

            for (int i=-windowSize; i<=windowSize; i++) {

                int sourceX = sourceCenterX+i;
                if (sourceX < 0)
                    sourceX = 0;
                else if (sourceX >= image.width())
                    sourceX = image.width() - 1;

                QRgb pixel = image.pixel(sourceX, y);

                float phase = sourceCenterFracX + i;

                float l = kernelValue(phase);

                sum += l;

                red += l*qRed(pixel);
                green += l*qGreen(pixel);
                blue += l*qBlue(pixel);
            }

            red /= sum;
            green /= sum;
            blue /= sum;

            cap(&red);
            cap(&green);
            cap(&blue);

            passImage.setPixel(QPoint(x, y), qRgb(red, green, blue));
        }

    for (int y=0; y<targetImage.height(); y++)
        for (int x=0; x<targetImage.width(); x++) {
            float red = 0, green = 0, blue = 0;

            float sourceCenterY = y*yRatio;

            float sourceCenterFracY = 1 - sourceCenterY + floor(sourceCenterY);

            float sum = 0;

            for (int i=-windowSize; i<=windowSize; i++) {

                int sourceY = sourceCenterY+i;
                if (sourceY < 0)
                    sourceY = 0;
                else if (sourceY >= passImage.height())
                    sourceY = passImage.height() - 1;

                QRgb pixel = passImage.pixel(x, sourceY);

                float phase = sourceCenterFracY + i;

                float l = kernelValue(phase);

                sum += l;

                red += l*qRed(pixel);
                green += l*qGreen(pixel);
                blue += l*qBlue(pixel);
            }

            red /= sum;
            green /= sum;
            blue /= sum;

            cap(&red);
            cap(&green);
            cap(&blue);

            targetImage.setPixel(QPoint(x, y), qRgb(red, green, blue));
        }

    return QuillImage(targetImage);
}
