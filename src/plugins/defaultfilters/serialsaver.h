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
  \class SerialSaver

  \brief Handles tiled JPEG saving for the save filter.

This class acts as an adaptation layer for LibJPEG for the purposes of
saving.
*/


#ifndef __QUILL_IMAGE_FILTER_HELPER_SERIAL_SAVER_H__
#define __QUILL_IMAGE_FILTER_HELPER_SERIAL_SAVER_H__

class QString;
class QSize;
class QuillImage;
class SerialSaverPrivate;

#include <QByteArray>

class SerialSaver
{
 public:
    SerialSaver(const QString &fileName, const QSize &fullImageSize,
                const QByteArray &rawExifData);
    ~SerialSaver();

    bool process(const QuillImage &image);
    bool isFinished() const;
    uchar *data(const QuillImage &image);

 private:
    void errorExit();

    SerialSaverPrivate *priv;
};

#endif // __QUILL_IMAGE_FILTER_HELPER_SERIAL_SAVER_H__
