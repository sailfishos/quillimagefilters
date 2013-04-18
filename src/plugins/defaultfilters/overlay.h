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
  \class Overlay

  \brief Helper filter to collect tiles into stripes required by save.

Each call to the Overlay filter paints one QuillImage (tile) on top of
another (horizontal stripe with the whole width of the full image).

Options supported: CropRectangle (the location of the stripe relative
to the full image) and Background (a QuillImage for supplying the
stripe). The tile will be supplied by a call to apply().

Default behavior: returns empty QuillImage.
*/

#ifndef __QUILL_FILTERS_OVERLAY_H__
#define __QUILL_FILTERS_OVERLAY_H__

#include "quillimagefilter.h"
#include "quillimagefilterimplementation.h"


class OverlayPrivate;

class Overlay : public QuillImageFilterImplementation
{
public:
    Overlay();
    ~Overlay();

    QuillImage apply(const QuillImage &image) const;

    bool setOption(const QString &option, const QVariant &value);
    QVariant option(const QString &option) const;
    const QStringList supportedOptions() const;

    virtual const QString name() const { return QuillImageFilter::Name_Overlay; }

    QuillImageFilter::QuillFilterRole role() const;

private:
    OverlayPrivate *priv;
};

#endif // __QUILL_FILTERS_OVERLAY_H__
