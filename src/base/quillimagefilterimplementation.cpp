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

#include <QVariant>
#include <QIODevice>
#include "quillimagefilterinterface.h"
#include "quillimagefilterimplementation.h"


QuillImageFilterImplementation::QuillImageFilterImplementation()
{
}

QuillImageFilterImplementation::~QuillImageFilterImplementation()
{
}

QSize QuillImageFilterImplementation::newFullImageSize(const QSize &fullImageSize) const
{
    return fullImageSize;
}

QRect QuillImageFilterImplementation::newArea(const QSize &fullImageSize, const QRect &area) const
{
    Q_UNUSED(fullImageSize);
    return area;
}

QVariant QuillImageFilterImplementation::option(const QString &filterOption) const
{
    Q_UNUSED(filterOption);
    return QVariant();
}

bool QuillImageFilterImplementation::setOption(const QString &filterOption, const QVariant &value)
{
    Q_UNUSED(filterOption);
    Q_UNUSED(value);
    return false;
}

QuillImage QuillImageFilterImplementation::apply(const QuillImage &image) const
{
    return image;
}

QIODevice* QuillImageFilterImplementation::iODevice() const
{
    return 0;
}

void  QuillImageFilterImplementation::setIODevice(QIODevice* iODevice)
{
    Q_UNUSED(iODevice);
    //It does nothing, the derived class will do
}

QuillImageFilter::ImageFilterError QuillImageFilterImplementation::error() const
{
    //It does nothing, the derived class will do
    return QuillImageFilter::NoError;
}

void QuillImageFilterImplementation::cancel()
{
    // Currently left blank
}

const QStringList QuillImageFilterImplementation::supportedOptions() const
{
    return QStringList();
}

QuillImageFilter::QuillFilterRole QuillImageFilterImplementation::role() const
{
    return QuillImageFilter::Role_Undefined;
}
