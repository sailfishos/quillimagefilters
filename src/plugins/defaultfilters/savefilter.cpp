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

#include <QFileInfo>
#include <QPainter>
#include <QIODevice>
#include <QFile>
#include <QDateTime>
#include "utime.h"

#include "quillimagefilter.h"
#include "serialsaver.h"
#include "savefilter.h"
#include "mimetype.h"

class SaveFilterPrivate
{
public:
    QString fileName, fileFormat, fileFormatQt;
    QImage fullImage;
    QSize fullImageSize;
    int tileCount;
    SerialSaver *serialSaver;
    //Other components own this memory
    QIODevice* iODevice;
    QuillImageFilter::ImageFilterError filterError;
    QByteArray rawExifData;
    QDateTime timeStamp;
};

SaveFilter::SaveFilter()
{
    priv = new SaveFilterPrivate;
    priv->fileName = "";
    priv->fileFormat = "";
    priv->fileFormatQt = "";
    priv->fullImage = QImage();
    priv->fullImageSize = QSize();
    priv->tileCount = 0;
    priv->serialSaver = 0;
    priv->iODevice = 0;
    priv->filterError = QuillImageFilter::NoError;
    priv->rawExifData = QByteArray();

    //m_role = QuillImageFilter::Role_Save;
}

SaveFilter::~SaveFilter()
{
    if (priv->serialSaver != 0)
        delete priv->serialSaver;
    delete priv;
}

QuillImage SaveFilter::apply(const QuillImage &image) const
{

    if(image.isNull())
        return QuillImage();
    // Simple format detection (1. given format 2. suffix)
    QString format = priv->fileFormatQt.toAscii();
    if (format.isEmpty())
        format = QFileInfo(priv->fileName).suffix().toAscii();

    // Jpeg always uses serial access saving, also for non-tiles,
    // to be able to insert EXIF data
    format = format.toLower();
    if ((format == "jpeg") || (format == "jpg"))
        return saveJpeg(image);

    // For non-tiles, just save
    if (!image.isFragment())
    {
        bool ok = saveFullImage(image);
        if (ok) {
            setFileModificationDateTime();
            return image;
        }
        else
            return QuillImage();
    }

    // First execute - prepare target

    if (priv->fullImageSize == QSize())
    {
            priv->fullImageSize = image.fullImageSize();

            priv->fullImage = QImage(priv->fullImageSize, QImage::Format_RGB32);
    }

    QPainter painter(&(priv->fullImage));
    painter.setCompositionMode(QPainter::CompositionMode_Source);

    painter.drawImage(image.area().topLeft(), image);

    priv->tileCount--;

    // Target full image has everything necessary, so do the final save.

    if (priv->tileCount == 0)
    {
        bool ok = saveFullImage(priv->fullImage);

        if (ok) {
            setFileModificationDateTime();
            // Return non-empty image to represent success
            return image;
        }
        else
            // Return empty image to represent failure
            return QuillImage();
    }
    else
        // No actual save was made, return non-empty image to represent success
        return image;
}

void SaveFilter::setFileModificationDateTime() const
{
    struct utimbuf times;
    times.actime = times.modtime = priv->timeStamp.toTime_t();
    utime(priv->fileName.toLocal8Bit().constData(), &times);
}

QuillImage SaveFilter::saveJpeg(const QuillImage &image) const
{
    // For thumbnail saving, set save target size to the thumbnail size
    QSize targetSize = image.fullImageSize();
    if (!image.isFragment())
        targetSize = image.size();

    if (priv->serialSaver == 0)
        priv->serialSaver = new SerialSaver(priv->fileName,
                                            targetSize,
                                            priv->rawExifData);

    bool result = priv->serialSaver->process(image);

    priv->tileCount--;

    setFileModificationDateTime();

    if (result)
        return image;
    else
        return QuillImage();
}

bool SaveFilter::setOption(const QString &option, const QVariant &value)
{
    bool bOK = true;

    if (option == QuillImageFilter::FileName)
        priv->fileName = value.toString();
    else if (option == QuillImageFilter::FileFormat) {
        priv->fileFormat = value.toString();
        priv->fileFormatQt = MimeType::toQImageFormat(priv->fileFormat);
    }
    else if (option == QuillImageFilter::TileCount)
        priv->tileCount = value.toInt();
    else if (option == QuillImageFilter::RawExifData)
        priv->rawExifData = value.toByteArray();
    else if (option == QuillImageFilter::Timestamp)
        priv->timeStamp = value.toDateTime();
    else
        bOK = false;

    return bOK;
}

QVariant SaveFilter::option(const QString &option) const
{
    if (option == QuillImageFilter::FileName)
        return QVariant(priv->fileName);
    else if (option == QuillImageFilter::FileFormat)
        return QVariant(priv->fileFormat);
    else if (option == QuillImageFilter::TileCount)
        return QVariant(priv->tileCount);
    else if ((option == QuillImageFilter::DisableUndo) ||
             (option == QuillImageFilter::DisablePreview))
        return QVariant(true);
    else if (option == QuillImageFilter::RawExifData)
        return QVariant(priv->rawExifData);
    else if (option == QuillImageFilter::Timestamp)
        return QVariant(priv->timeStamp);
    else
        return QVariant();
}

const QStringList SaveFilter::supportedOptions() const
{
    return QStringList() << QuillImageFilter::FileName
                         << QuillImageFilter::FileFormat
                         << QuillImageFilter::DisableUndo
                         << QuillImageFilter::DisablePreview
                         << QuillImageFilter::TileCount
                         << QuillImageFilter::RawExifData
                         << QuillImageFilter::Timestamp;
}

QIODevice* SaveFilter::iODevice() const
{
    return priv->iODevice;
}

void SaveFilter::setIODevice(QIODevice* iODevice)
{
    priv->iODevice = iODevice;
}

bool SaveFilter::saveFullImage(const QuillImage &image) const
{
    QImageWriter *writer;
    if (priv->iODevice)
        writer = new QImageWriter(priv->iODevice, priv->fileFormatQt.toAscii());
    else
        writer = new QImageWriter(priv->fileName);

    if (!priv->fileFormatQt.isNull())
        writer->setFormat(priv->fileFormatQt.toAscii());

    bool ok = writer->write(image);

    if (!ok)
        const_cast<SaveFilter *>(this)->setError(writer->error());

    delete writer;
    return ok;
}

void SaveFilter::setError(QImageWriter::ImageWriterError error)
{
    switch(error){
    case QImageWriter::DeviceError:
        priv->filterError = QuillImageFilter::UnspecifiedError;
        break;
    case QImageWriter::UnsupportedFormatError:
        priv->filterError = QuillImageFilter::UnsupportedFormatError;
        break;
    case QImageWriter::UnknownError:
        priv->filterError = QuillImageFilter::UnspecifiedError;
        break;
    default:
        break;
    }
}

QuillImageFilter::QuillFilterRole SaveFilter::role() const
{
    return QuillImageFilter::Role_Save;
}

