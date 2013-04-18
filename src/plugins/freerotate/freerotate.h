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
  \class FreeRotate

  \brief A free rotate / straighten horizon filter with autocrop.

Straightens the image by a low angle, autocrops the image
afterwards. May break tile boundaries, and as a result tiles may
become partially transparent on top of each other.

Supports options: Angle (-15 to +15).

Default behavior: does nothing.
*/

#ifndef __QUILL_FILTERS_ROTATE_FREE_H__
#define __QUILL_FILTERS_ROTATE_FREE_H__

#include "quillimagefilterimplementation.h"

class FreeRotatePrivate;

class FreeRotate : public QuillImageFilterImplementation
{
public:
    FreeRotate();
    ~FreeRotate();

    QuillImage apply(const QuillImage &image) const;

    QSize newFullImageSize(const QSize &fullImageSize) const;
    QRect newArea(const QSize &fullImageSize, const QRect &area) const;

    bool setOption(const QString &option, const QVariant &value);
    QVariant option(const QString &option) const;
    const QStringList supportedOptions() const;

    virtual const QString name() const { return QuillImageFilter::Name_FreeRotate; }

private:
    /*!
      Pre-calculates some of the computationally complex stuff.
     */

    void calculateCutoffs(const QSize &fullImageSize);

    FreeRotatePrivate *priv;
};

#endif
