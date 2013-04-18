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
  \class QuillImage

  \brief Represents a full-resolution image, a lower-resolution
  thumbnail, or part of an image.

QuillImage is an extension to QImage, containing information about the
dimensions of the full image represented by it, as in fullImageSize(),
and the relative location of the QuillImage within the full image
coordinates, as in area(). As with QImage, the data in QuillImage
objects is implicitly shared.
*/

#ifndef __QUILL_IMAGE_H__
#define __QUILL_IMAGE_H__

#include <QImage>

class QSize;
class QRect;
class QuillImagePrivate;

class QuillImage : public QImage
{
public:
    QuillImage();
    QuillImage(const QImage &image);
    QuillImage(const QImage &image, const QSize &fullImageSize);
    QuillImage(const QuillImage &image);
    QuillImage(const QuillImage &quillImage, const QImage &image);

    virtual ~QuillImage();

    /*!
      The size (in pixels) of the full image,
      of which this image is a part.
    */

    QSize fullImageSize() const;

    /*!
      Change the size (in pixels) of the full image,
      of which this image is a part.
    */

    void setFullImageSize(const QSize &size);

    /*!
      Returns true if the image is a fragment, false if the
      image is the full image or a downscaled version of it.
     */

    bool isFragment() const;

    /*!
      Returns the area covered by this image, in full-image coordinates.
    */

    QRect area() const;

    /*!
      Sets the area covered by this image, in full-image coordinates.
    */

    void setArea(const QRect &area);

    /*!
      Returns the z-coordinate of this image. This is useful for
      determining which of the potentially overlapping images to draw first.
    */

    int z() const;

    /*!
      Sets the z-coordinate of this image. This is useful for
      determining which of the potentially overlapping images to draw first.
    */

    void setZ(int z);

    /*!
      Used to check the target size of the image. This is usually the
      same as the image size. See also setTargetSize().
     */

    QSize targetSize() const;

    /*!
      Used to set the target size if this QuillImage is passed as a
      parameter to an image filter, currently only used by the Load filter.
      The target size defaults to the image size.
    */

    void setTargetSize(const QSize &size);

    QuillImage &operator=(const QuillImage &other);

private:
    QuillImagePrivate *priv;

};

#endif // __QUILL_IMAGE_H__
