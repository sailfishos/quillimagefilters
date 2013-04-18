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
  \class QuillImageFilterGenerator

  \brief Filters which generate another filters.

Generators are a special type of QuillImageFilter. Instead of applying
its changes to an image as other filters do, a Generator will make
observations from an image and instead of correcting the observed
problems itself will return a filter which will make the
corrections.

This is especially useful with LibQuill, where the same filter is run
many times for different resolutions and tiles. Instead of always
running for example the AutoContrast detection differently for each
tile (which would waste effort and produce a silly result), the
detection is run only once for a low-resolution representation of the
image, and the resulting filter is afterwards applied separately to
each tile.
 */

#ifndef __QUILL_IMAGE_FILTER_GENERATOR_H__
#define __QUILL_IMAGE_FILTER_GENERATOR_H__

#include "quillimagefilter.h"

class QuillImageFilterGeneratorPrivate;

class QuillImageFilterGenerator : public QuillImageFilter
{
public:
    /*!
      Creates a new filter generator. Filter generators are filters
      which return other filters.
     */

    QuillImageFilterGenerator(const QString &name);

    virtual ~QuillImageFilterGenerator();

    /*!
      Applies the generator to a given preview or fragment.
     */

    QuillImage apply(const QuillImage &image) const;

    /*!
      Returns the resulting filter of the generator to be used later.
     */

    QuillImageFilter *resultingFilter() const;

    /*!
      Provides a hint for libquill - do we run this generator
      always on the first preview level (default), or on the best
      available one?
    */

    virtual bool isUsedOnPreview() const;

 private:
    QuillImageFilterGeneratorPrivate *priv;
};


#endif // __QUILL_IMAGE_FILTER_GENERATOR_H__
