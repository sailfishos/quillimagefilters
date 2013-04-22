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

#include <QImageReader>

#include "mimetype.h"

QMap<QString,QString> MimeType::map;

void MimeType::init()
{
    map.insert(QLatin1String("image/jpeg"), QLatin1String("jpg"));
    map.insertMulti(QLatin1String("image/jpeg"), QLatin1String("jpeg"));
    map.insert(QLatin1String("image/png"), QLatin1String("png"));
    map.insert(QLatin1String("image/gif"), QLatin1String("gif"));
    map.insert(QLatin1String("image/bmp"), QLatin1String("bmp"));
    map.insert(QLatin1String("image/vnd.wap.wbmp"), QLatin1String("wbmp"));
    map.insert(QLatin1String("image/tiff"), QLatin1String("tiff"));
    map.insert(QLatin1String("image/x-xpixmap"), QLatin1String("xpm"));
    map.insert(QLatin1String("image/svg+xml"), QLatin1String("svg"));
    map.insert(QLatin1String("image/ppm"), QLatin1String("ppm"));
    map.insert(QLatin1String("image/xbm"), QLatin1String("xbm"));
    map.insert(QLatin1String("image/mng"), QLatin1String("mng"));
    map.insert(QLatin1String("image/pbm"), QLatin1String("pbm"));
}

QString MimeType::toQImageFormat(const QString &mimeType)
{
    if (map.isEmpty())
        init();

    QString format = map.value(mimeType);

    if (!format.isNull())
        return format;
    else if (mimeType.startsWith(QLatin1String("image/")))
        // Try to auto-generate a format from the mimetype
        return mimeType.mid(6);
    else if (QImageReader::supportedImageFormats().
             contains(mimeType.toLatin1()))
        return mimeType;
    else return QString();
}

QString MimeType::fromQImageFormat(const QString &format)
{
    if (map.isEmpty())
        init();

    return map.key(format);
}
