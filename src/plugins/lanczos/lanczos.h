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

/*!
  \class Lanczos

  \brief Applies Lanczos rescaling to the image.

This version will only work with upscaling. It is intended to be used
with small-size images only, as it is not well optimized and does not
behave correctly with tiling.

Supports options: SizeAfter (target size),
                  Radius (Lanczos kernel size, valid values are 2 and 3.)

Default behavior: no operation. If only SizeAfter is defined, Radius
defaults to 3.
*/

#ifndef LANCZOS_H
#define LANCZOS_H

#include "quillimagefilterimplementation.h"

class Lanczos : public QuillImageFilterImplementation
{
public:
    Lanczos();
    ~Lanczos();

    QuillImage apply(const QuillImage &image) const;

    bool setOption(const QString &option, const QVariant &value);
    QVariant option(const QString &option) const;
    const QStringList supportedOptions() const;

    virtual const QString name() const { return QuillImageFilter::Name_Lanczos; }

 private:
    static float sinc(float x);
    float lanczosFunction(float x);

    void createKernel();
    float kernelValue(float x) const;
    static void cap (float *x);

 private:
    int windowSize;
    float *kernel;
    QSize targetSize;
};

#endif
