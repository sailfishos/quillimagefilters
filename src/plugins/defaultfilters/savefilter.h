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
  \class SaveFilter

  \brief A pseudo-filter for saving images.

Saves its input image to the file system or if iODevice() is set, to
the given IO device. Apply() will return an empty image on failure, or
a non-empty image on success.

Supports options: FileName, FileFormat (as in QImageReader).

Default behavior: does nothing (if iODevice() is specified, writes
input to that file).
*/

#ifndef __QUILL_FILTERS_SAVE_H__
#define __QUILL_FILTERS_SAVE_H__

#include <QImageWriter>
#include "quillimagefilter.h"
#include "quillimagefilterimplementation.h"

class QIODevice;
class SaveFilterPrivate;

class SaveFilter : public QuillImageFilterImplementation
{
public:
    SaveFilter();
    ~SaveFilter();

    /*!
      Applies the save operation.

      @return a non-empty QuillImage to represent success, an empty
      QuillImage to represent failure.
     */

    QuillImage apply(const QuillImage &image) const;

    bool setOption(const QString &option, const QVariant &value);
    QVariant option(const QString &option) const;
    const QStringList supportedOptions() const;

    const QString name() const { return QuillImageFilter::Name_Save; }

    QuillImageFilter::QuillFilterRole role() const;

    QIODevice* iODevice() const;

    void setIODevice(QIODevice* iODevice);
private:
    SaveFilterPrivate *priv;

    void setFileModificationDateTime() const;

    QuillImage saveJpeg(const QuillImage &image) const;

    bool saveFullImage(const QuillImage &image) const;

    void setError(QImageWriter::ImageWriterError error);

private:
    static const QLatin1String Option_Timestamp;
};

#endif // __QUILL_FILTERS_SAVE_H__
