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

#ifndef __QUILLIMAGEFILTERIMPLEMENTATION_H__
#define __QUILLIMAGEFILTERIMPLEMENTATION_H__

#include <QVariant>
#include <QStringList>

#include "quillimage.h"
#include "quillimagefilterinterface.h"

/*!
  \class QuillImageFilterImplementation

  \brief Default superclass and implementation for an internal filter
  (handler) which is delivered by a
  QuillImageFilterInterface::create().

  Instances created directly from this class will work as an identity
  filter (one which returns exactly the same image or dimensions that
  were provided). They do not need to be reimplemented if a filter
  developer wants to implement a simple filter.
*/

class QuillImageFilter;

class QuillImageFilterImplementation
{
public:

    /*!
      Initializes common things to all filters.
     */

    QuillImageFilterImplementation();

    /*!
      Frees common things to all filters.
     */

    virtual ~QuillImageFilterImplementation();

    /*!
      Returns the value of a filter option. These options are the
      recommended way to pass information to the filters. This
      function needs to be reimplemented in a derived class to support
      options.
     */

    virtual QVariant option(const QString &filterOption) const;

    /*!
      Sets the value of a filter option. These options are the recommended
      way to pass information to the filters. This
      function needs to be reimplemented in a derived class to support
      options.
     */

    virtual bool setOption(const QString &filterOption, const QVariant &value);

    /*!
      Applies the filter to a given preview or fragment.
     */

    virtual QuillImage apply(const QuillImage &image) const;

    /*!
      If the filter changes dimensions of the image,
      this can be used to re-calculate a new size for the full image.
     */

    virtual QSize newFullImageSize(const QSize &fullImageSize) const;

    /*!
      If the filter changes dimensions of the image,
      this can be used to re-calculate a new position for a specific tile.
    */

    virtual QRect newArea(const QSize &fullImageSize, const QRect &area) const;

    /*!
      Returns the IO device used, see setIODevice().
     */
    virtual QIODevice* iODevice() const;

    /*!
      Sets the IO device used. This is only used by filters which
      represent file system access (loading and saving). The device
      remains property of the caller but remains in use by the filter;
      it should only be deleted after the filter has been removed.
    */
    virtual void setIODevice(QIODevice* iODevice);

    /*!
      Note: This is currently an API placeholder with no actual
      functionality.

      Cancel a call to apply() which is currently running in some
      other thread. This is the only method of its class which is
      thread safe. Cancellation implementation by filter is
      optional. When the canceled filter recognizes the cancellation,
      it will terminate gracefully, return an invalid QuillImage, and
      have its error() value set as QuillImageFilter::CanceledError.
    */
    virtual void cancel();

    /*!
     */
    virtual QuillImageFilter::ImageFilterError error() const;

    virtual const QString name() const { return QString(); }


    virtual QuillImageFilter::QuillFilterRole role() const;

    virtual const QStringList supportedOptions() const;


private:

    Q_DISABLE_COPY(QuillImageFilterImplementation)
};


#endif // __QUILLIMAGEFILTERIMPLEMENTATION_H__
