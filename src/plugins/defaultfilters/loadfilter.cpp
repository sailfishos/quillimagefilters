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

#include <QIODevice>
#include <QFile>
#include <QColor>
#include <QPainter>
#include <QuillMetadata>
#include "quillimagefilter.h"
#include "loadfilter.h"
#include "mimetype.h"



class LoadFilterPrivate
{
public:
    LoadFilterPrivate():
        fileName(),
        fileFormat(),
        fileFormatQt(),
        fullSize(),
        iODevice(0),
        filterError(QuillImageFilter::NoError),
        backgroundColor(Qt::black),
        orientation(0),
        ignoreExifOrientation(false),
        isInvalid(false),
        bufferInitialized(false)
    {}

    QString fileName;
    QString fileFormat;
    QString fileFormatQt;
    QSize fullSize;
    // Other components will own it
    QIODevice* iODevice;
    QuillImageFilter::ImageFilterError filterError;
    QColor backgroundColor;
    int orientation;
    bool ignoreExifOrientation;
    bool isInvalid;

    // Whether static LoadFilter::buffer was initialized for use with this
    // particular filter instance. This is used to ensure that we don't use
    // the buffer with the contents from other instance, as filterAddress
    // might be the same as for a deleted LoadFilter.
    bool bufferInitialized;
};


//
// Those static variables are only used in:
//
//   - LoadFilter::apply()
//
// LoadFilter::apply() will be always called from the BackgroundThread,
// which runs only one filter at a time, so there is no possibility of
// having two instances accessing them at the same time from the apply()
// method. This means they need no locking.
//

LoadFilter*   LoadFilter::filterAddress  = 0;
QImageReader* LoadFilter::reader         = 0;
QImage        LoadFilter::fullImage      = QImage();
QBuffer       LoadFilter::buffer;


LoadFilter::LoadFilter():
    priv(new LoadFilterPrivate())
{
}

LoadFilter::~LoadFilter()
{
    delete priv;
}

void LoadFilter::detectFormat()
{
    // FIXME: work-around bug 242369
    if (priv->fileFormatQt.isEmpty() &&
        priv->fileFormat.isEmpty() &&
        priv->fileName.endsWith(".mp4")) {
        if (priv->filterError == QuillImageFilter::NoError)
            priv->filterError = QuillImageFilter::UnsupportedFormatError;
        priv->isInvalid = true;
        priv->fileFormat = "video/mp4";
        return;
    }

    if (priv->fileFormatQt.isEmpty() &&
        (priv->fileFormat.isEmpty() || priv->fileFormat.startsWith("image/"))) {
        QImageReader reader;
        if (!priv->iODevice)
            reader.setFileName(priv->fileName);
        else
            reader.setDevice(priv->iODevice);

        priv->fileFormatQt = formatFromReader(&reader);
    }

    if (priv->fileFormat.isEmpty()) {
        priv->fileFormat = MimeType::fromQImageFormat(priv->fileFormatQt);
    }

    if (priv->fileFormatQt.isEmpty()) {
        if (priv->filterError == QuillImageFilter::NoError)
            priv->filterError = QuillImageFilter::UnsupportedFormatError;
        priv->isInvalid = true;
    }
}

QuillImage LoadFilter::apply(const QuillImage &tile) const
{
    if (priv->fileFormatQt.isEmpty() && !priv->isInvalid)
        const_cast<LoadFilter *>(this)->detectFormat();

    if (priv->isInvalid)
        return QuillImage();

    bool isNew = !reader
            || (filterAddress != this)
            || !priv->bufferInitialized
            || !tile.isFragment()
            || (priv->fileFormatQt != reader->format())
            || (!tile.targetSize().isEmpty()
            && (tile.targetSize() != tile.area().size()));

    filterAddress = const_cast<LoadFilter*>(this);
    priv->bufferInitialized = true;

    if (priv->iODevice) {
        delete reader;
        reader = new QImageReader(priv->iODevice, priv->fileFormatQt.toLatin1());

        if (!priv->fileFormatQt.isEmpty())
            reader->setFormat(priv->fileFormatQt.toLatin1());
    }
    else {
        if (isNew) {
            delete reader;
            reader = 0;

            if (!const_cast<LoadFilter*>(this)->readFileToByteArray())
                return QuillImage();

            buffer.seek(0);
            priv->orientation = readOrientation();
            reader = new QImageReader(&buffer, priv->fileFormatQt.toLatin1());

            if (!priv->fileFormatQt.isEmpty())
                reader->setFormat(priv->fileFormatQt.toLatin1());
        }
        else {
            buffer.seek(0);
        }
    }

    QuillImage input = tile;

    if (input.fullImageSize().isEmpty())
        input.setFullImageSize(reader->size());

    if (input.area().isEmpty())
        input.setArea(QRect(QPoint(0, 0), input.fullImageSize()));

    QImage newImage;

    QSize targetSize = input.targetSize();
    QSize fullImageSize = input.fullImageSize();

    QRect area = rotateArea(fullImageSize, input.area(), input);

    if ((priv->orientation == Exif_Orientation_Rotated90) ||
        (priv->orientation == Exif_Orientation_Rotated270)) {
        targetSize.transpose();
        fullImageSize.transpose();
    }

    /*
       Resetting reader for ScaledSize and ClipRect. This resetting
       will make sure that the reader will have all the options set
       correctly if the same reader has been used previously.
       Does not do the calculation in all cases since it is time consuming.
    */

    if (reader->scaledSize().isValid() || reader->clipRect().isValid()) {
        QSize readerSize = reader->size();
        reader->setScaledSize(readerSize);
        reader->setClipRect(QRect(QPoint(0,0),readerSize));
    }

    if (!input.isFragment()){
        fullImage = QImage();

        if (!targetSize.isEmpty()) {
            reader->setScaledSize(targetSize);
        }

        newImage = readFromReader();
    }
    else if (!targetSize.isEmpty() &&
             (targetSize != area.size())) {
        // Both cropping and scaling have been requested
        // Current implementation: ask plugin for the scaled image, then crop
        // This can be later optimized with a plugin that effectively
        // supports the ScaledClipRect option.
        reader->setScaledSize(QSize(targetSize.width() *
                                    fullImageSize.width() /
                                    area.width(),
                                    targetSize.height() *
                                    fullImageSize.height() /
                                    area.height()));

        newImage = readFromReader();

        newImage = newImage.copy(input.area().left() *
                                 input.targetSize().width() /
                                 input.area().width(),
                                 input.area().top() *
                                 input.targetSize().height() /
                                 input.area().height(),
                                 input.targetSize().width(),
                                 input.targetSize().height());

        newImage = QuillImage(input, newImage);
    }
    else if (reader->supportsOption(QImageIOHandler::ClipRect) &&
             // Standard Qt now supports ClipRect with a too slow
             // implementation, so we use the ScaledClipRect information
             // to reject the standard Qt Jpeg plugin.
             // TODO: This needs to be removed if Qt becomes faster.
             !reader->supportsOption(QImageIOHandler::ScaledClipRect)) {

        fullImage = QImage();

        //if it is real a tile, we then reset reader, otherwise we consider it as a full image
        if(!area.isNull()){
            //set the scale size again here because preview sets the preview scale size already.
            reader->setScaledSize(area.size());
            // clip rect support in image reader
            reader->setClipRect(area);
        }

        newImage = readFromReader();
    }
    else {
        // no clip rect support: preserve whole full image as a tile cache,
        // uses a lot of extra memory
        if (isNew || fullImage.isNull())
            fullImage = readFromReader();

        newImage = fullImage.copy(input.area());
    }

    QImage convertedImage;
    if (newImage.hasAlphaChannel()){
        convertedImage = renderAlpha(newImage, priv->backgroundColor);
    }
    else{
        convertedImage = newImage.convertToFormat(QImage::Format_RGB32);
    }

    QuillImage ret_value;

    if (!input.targetSize().isEmpty() || !input.fullImageSize().isEmpty() ||
        !input.area().isEmpty()) {
        ret_value = QuillImage(input, convertedImage);
    }
    else{
        // full image
        ret_value = QuillImage(convertedImage);
    }

    if (priv->iODevice) {
        delete reader;
        reader = 0;
    }

    return ret_value;
}

QImage LoadFilter::readFromReader() const
{
    QImage image = reader->read();

    if (image.isNull()){
        const_cast<LoadFilter*>(this)->setError(translateReaderError(reader->error()));
    }

    switch (priv->orientation) {
    case Exif_Orientation_Rotated180 :
    {
        image = image.transformed(QTransform().rotate(180));
        break;
    }
    case Exif_Orientation_Rotated270 :
    {
        image = image.transformed(QTransform().rotate(-90));
        break;
    }
    case Exif_Orientation_Rotated90 :
    {
        image = image.transformed(QTransform().rotate(90));
        break;
    }
    default:
        break;
    }

    return image;
}

QByteArray LoadFilter::formatFromReader(QImageReader *reader)
{
    QByteArray format = reader->format();
    if (format.isEmpty())
        setError( translateReaderError(reader->error()) );

    return format;
}

QSize LoadFilter::newFullImageSize(const QSize &fullImageSize) const
{
    Q_UNUSED(fullImageSize);

    if (priv->fileFormatQt.isEmpty() && !priv->isInvalid)
        const_cast<LoadFilter *>(this)->detectFormat();

    if (priv->isInvalid)
        return QSize();

    QImageReader reader(priv->fileName, priv->fileFormatQt.toLatin1());
    if(priv->iODevice)
        reader.setDevice(priv->iODevice);

    priv->fullSize = reader.size();

    // QSize::isEmpty() also return true for invalid sizes
    if (priv->fullSize.isEmpty())
        const_cast<LoadFilter*>(this)->setError(translateReaderError(reader.error()));

    priv->orientation = readOrientation();
    if ((priv->orientation == Exif_Orientation_Rotated90) ||
        (priv->orientation == Exif_Orientation_Rotated270))
        priv->fullSize.transpose();

    return priv->fullSize;
}

bool LoadFilter::setOption(const QString &option, const QVariant &value)
{
    bool bOK = true;

    if (option == QuillImageFilter::FileName) {
        priv->isInvalid = false;
        priv->fileName = value.toString();
    }
    else if (option == QuillImageFilter::FileFormat) {
        priv->isInvalid = false;
        priv->fileFormatQt = value.toString();
        priv->fileFormat = MimeType::fromQImageFormat(priv->fileFormatQt);
    }
    else if (option == QuillImageFilter::MimeType) {
        priv->isInvalid = false;
        priv->fileFormat = value.toString();
        priv->fileFormatQt = MimeType::toQImageFormat(priv->fileFormat);
    }
    else if (option == QuillImageFilter::BackgroundColor)
        priv->backgroundColor = value.value<QColor>();
    else if (option == QuillImageFilter::IgnoreExifOrientation)
        priv->ignoreExifOrientation = value.toBool();
    else
        bOK = false;

    return bOK;
}

QVariant LoadFilter::option(const QString &option) const
{
    if (option == QuillImageFilter::FileName)
        return QVariant(priv->fileName);
    else if (option == QuillImageFilter::FileFormat) {
        if (priv->fileFormatQt.isEmpty() && !priv->isInvalid)
            const_cast<LoadFilter *>(this)->detectFormat();
        return QVariant(priv->fileFormatQt);
    }
    else if (option == QuillImageFilter::MimeType) {
        if (priv->fileFormatQt.isEmpty() && !priv->isInvalid)
            const_cast<LoadFilter *>(this)->detectFormat();
        return QVariant(priv->fileFormat);
    }
    else if (option == QuillImageFilter::DisableUndo)
        return QVariant(true);
    else if (option == QuillImageFilter::SizeAfter)
        return QVariant(priv->fullSize);
    else if (option == QuillImageFilter::BackgroundColor)
        return QVariant(priv->backgroundColor);
    else if (option == QuillImageFilter::IgnoreExifOrientation)
        return QVariant(priv->ignoreExifOrientation);
    else
        return QVariant();
}


const QStringList LoadFilter::supportedOptions() const
{
    return QStringList() << QuillImageFilter::FileName
                         << QuillImageFilter::FileFormat
                         << QuillImageFilter::BackgroundColor
                         << QuillImageFilter::MimeType
                         << QuillImageFilter::DisableUndo
                         << QuillImageFilter::SizeAfter
                         << QuillImageFilter::IgnoreExifOrientation;
}

QIODevice* LoadFilter::iODevice() const
{
    return priv->iODevice;
}

void LoadFilter::setIODevice(QIODevice* iODevice)
{
    priv->iODevice = iODevice;
}

QuillImageFilter::ImageFilterError LoadFilter::translateReaderError(QImageReader::ImageReaderError errorCode) const
{
   QuillImageFilter::ImageFilterError result;
   switch(errorCode){
   case QImageReader::FileNotFoundError:
       result= QuillImageFilter::FileNotFoundError;
       break;
   case QImageReader::UnsupportedFormatError:
       result = QuillImageFilter::UnsupportedFormatError;
       break;
       // Due to a Qt bug, size query on a corrupted file after explicitly
       // setting a format gives unknown error
   case QImageReader::UnknownError:
   case QImageReader::InvalidDataError:
       result = QuillImageFilter::InvalidDataError;
       break;
   default:
       result =QuillImageFilter::UnspecifiedError;
       break;
   }
   return result;
}

void LoadFilter::setError(QuillImageFilter::ImageFilterError error)
{
    priv->filterError = error;
}

QuillImageFilter::ImageFilterError LoadFilter::error() const
{
    return priv->filterError;
}

QuillImage LoadFilter::renderAlpha(const QuillImage &image,
                                   const QColor &backgroundColor)
{
    QuillImage target(image, QImage(image.size(), QImage::Format_RGB32));
    target.fill(backgroundColor.rgb());
    QPainter painter(&target);
    painter.drawImage(QPoint(0, 0), image);
    return target;
}

int LoadFilter::readOrientation() const
{
    if (priv->ignoreExifOrientation)
        return 0;
    QuillMetadata metadata(priv->fileName, QuillMetadata::ExifFormat,
                           QuillMetadata::Tag_Orientation);
    return metadata.entry(QuillMetadata::Tag_Orientation).toInt();
}

QRect LoadFilter::rotateArea(const QSize &fullImageSize, const QRect &area, const QuillImage &tile) const
{
    switch (priv->orientation) {
    case Exif_Orientation_Normal:
        return area;
    case Exif_Orientation_Rotated180:
        return QRect(fullImageSize.width()-area.right() - 1,
                     fullImageSize.height()-area.bottom() - 1,
                     area.width(), area.height());
    case Exif_Orientation_Rotated270:
        if(tile.isFragment())
            return QRect(fullImageSize.height()-area.bottom()-1,
                               area.left(),
                               area.height(), area.width());

        else
            return QRect(area.top(),
                         fullImageSize.width()-area.right() - 1,
                         area.height(), area.width());
    case Exif_Orientation_Rotated90:
        //The used base image is 3840x2160(WxH), we need to map from 2160x3840(WxH) when we use tiles
        if(tile.isFragment())
            return QRect(area.top(),
                         fullImageSize.width()-area.right() - 1,
                         area.height(), area.width());

        //for full screen display from the full image
        else
            return QRect(fullImageSize.height()-area.bottom() - 1,
                           area.left(),
                           area.height(), area.width());
    default:
        return area;
    }
}

QuillImageFilter::QuillFilterRole LoadFilter::role() const
{
    return QuillImageFilter::Role_Load;
}

bool LoadFilter::readFileToByteArray()
{
    bool success(false);
    QFile file(priv->fileName);

    buffer.close();
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray bytes(file.readAll());
        if (bytes.isEmpty()) {
            buffer.setData(0, 0);
            const_cast<LoadFilter*>(this)->setError(translateReaderError(QImageReader::FileNotFoundError));
        }
        else {
            buffer.setData(bytes);
            success = true;
        }
    }
    else if (!file.exists()) {
        const_cast<LoadFilter*>(this)->setError(translateReaderError(QImageReader::FileNotFoundError));
        buffer.setData(0, 0);
    }
    else {
        const_cast<LoadFilter*>(this)->setError(QuillImageFilter::FileCanNotOpenError);
        buffer.setData(0, 0);
    }

    buffer.open(QIODevice::ReadOnly);
    return success;
}

void LoadFilter::cleanup()
{
    // Get rid of statically allocated stuff. This helps applications
    // using Quill to save memory when they request a cleanup to be done.
    delete LoadFilter::reader;
    LoadFilter::reader = 0;
    LoadFilter::filterAddress = 0;
    LoadFilter::fullImage = QImage();
    LoadFilter::buffer.setData(0, 0);
}
