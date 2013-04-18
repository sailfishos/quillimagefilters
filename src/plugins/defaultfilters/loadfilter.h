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
  \class LoadFilter

  \brief A pseudo-filter for loading images.

Loads an image from the file system or if iODevice() is set, from the
given IO device. Any input QuillImage is treated as a request for an
area and resolution of interest.

Supports options: FileName, FileFormat (as in QImageReader).

Default behavior: does nothing (if iODevice() is specified, reads
full image from the device).
*/

#ifndef __QUILL_FILTERS_LOAD_H__
#define __QUILL_FILTERS_LOAD_H__

#include <QImageReader>
#include <QBuffer>
#include <QByteArray>
#include "quillimagefilter.h"
#include "quillimagefilterimplementation.h"

class QIODevice;
class LoadFilterPrivate;
class QuillImageFilterError;
Q_EXTERN_C void quillimagefilter_plugin_cleanup();


class LoadFilter : public QuillImageFilterImplementation
{
public:
    LoadFilter();
    ~LoadFilter();

    /*!
      Loads the given tile or preview.
      @param tile the tile, ignores any image data.
    */

    QuillImage apply(const QuillImage &tile) const;

    /*!
      Gets the full image size of the image (re-implementation.)
    */

    QSize newFullImageSize(const QSize &fullImageSize) const;

    bool setOption(const QString &option, const QVariant &value);
    QVariant option(const QString &option) const;
    const QStringList supportedOptions() const;

    QIODevice* iODevice() const;
    void setIODevice(QIODevice* iODevice);
    QuillImageFilter::ImageFilterError error() const;


    virtual const QString name() const { return QuillImageFilter::Name_Load; }

    QuillImageFilter::QuillFilterRole role() const;

    static void cleanup();

private:
    void detectFormat();
    void setError(QuillImageFilter::ImageFilterError error);
    QuillImageFilter::ImageFilterError
        translateReaderError(QImageReader::ImageReaderError errorCode) const;
    QImage readFromReader() const;

    QByteArray formatFromReader(QImageReader *reader);
    bool readFileToByteArray();

    static QuillImage renderAlpha(const QuillImage &image,
                                  const QColor &backgroundColor);

    int readOrientation() const;
    QRect rotateArea(const QSize &fullImageSize, const QRect &area,const QuillImage &tile) const;

private:
    LoadFilterPrivate *priv;
    static QImageReader *reader;
    static QBuffer buffer;
    static QImage fullImage;
    static LoadFilter *filterAddress;

private:
    static const int Exif_Orientation_Normal = 1;
    static const int Exif_Orientation_Rotated90 = 6;
    static const int Exif_Orientation_Rotated180 = 3;
    static const int Exif_Orientation_Rotated270 = 8;
};

#endif // __QUILL_FILTERS_LOAD_H__
