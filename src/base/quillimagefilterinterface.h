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


#ifndef __QUILL_IMAGE_FILTER_INTERFACE_H__
#define __QUILL_IMAGE_FILTER_INTERFACE_H__


#include <QVariant>
#include <QStringList>
#include <QtPlugin>

#include "quillimagefilter.h"

/*!
  \class QuillImageFilterInterface

  \brief The plugin interface for plugins delivering image filter
  implementations.

  Plugin classes containing one or more image filters implementations
  need to implement this interface.
*/


class QuillImageFilterImplementation;

class QuillImageFilterInterface
{
 public:
    /*!
      Virtual destructor for plugins
     */

    virtual ~QuillImageFilterInterface() {}

    /*!
      Creates an instance for an actual image filter implementation.

      \param @name Name of the filter implementation.
     */

    virtual QuillImageFilterImplementation *create(const QString &name) = 0;

    /*!
      Creates an instance for an actual image filter implementation, given a role.

      \param @role Role of the filter implementation.
     */
    virtual QuillImageFilterImplementation *create(QuillImageFilter::QuillFilterRole role)
    {
        Q_UNUSED(role);
        return 0;
    }

    /*!
      Returns a list of all image filter implementation names that can be
      created by the plugin.
     */

    virtual const QStringList name() const = 0;

    /*!
      Returns a list of all supported image filter special roles. See
      QuillImageFilter::role() for an explanation of special roles.
     */


    const virtual QList<QuillImageFilter::QuillFilterRole> role() const
    {
        return QList<QuillImageFilter::QuillFilterRole>();
    }

};

Q_DECLARE_INTERFACE(QuillImageFilterInterface,
                    "com.meego.Quill.QuillImageFilterInterface/1.0")

#endif //__QUILL_IMAGE_FILTER_INTERFACE_H__
