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

#include <cmath>
#include <QImage>

#include "quillimagefilter.h"
#include "freerotate.h"

class FreeRotatePrivate
{
public:
    float angle;
    QSize fullImageSize;
    QSize newFullImageSize;
    QTransform transform;
    int cropW, cropH;		// Amount of cropping: new width is oldWidth-2*cropW
    int translateW, translateH; // Amount of translation when whole image is rotated
};

FreeRotate::FreeRotate()
{
    priv = new FreeRotatePrivate;
    priv->angle = 0.0;
    priv->cropW = 0;
    priv->cropH = 0;
    priv->translateW = 0;
    priv->translateH = 0;
    priv->fullImageSize = QSize(0, 0);
    priv->transform = QTransform();
    priv->newFullImageSize = QSize();
}

FreeRotate::~FreeRotate()
{
    delete priv;
}

QuillImage FreeRotate::apply(const QuillImage &image) const
{
    if (image.fullImageSize().isEmpty())
    	return QuillImage();

    ((FreeRotate*)this)->calculateCutoffs(image.fullImageSize());

    QTransform transform = priv->transform;

    if (!image.isFragment()) {
	transform = QTransform().rotate(priv->angle);

	const QImage rotatedImage =
	    image.transformed(transform);

	int croppedHeight = image.height() -
		      ((2.0 * priv->cropH * image.height()) / image.fullImageSize().height());
	int croppedWidth = image.width() -
		      ((2.0 * priv->cropW * image.width()) / image.fullImageSize().width());

	QuillImage croppedImage =
	    rotatedImage.copy((rotatedImage.width()  - croppedWidth)/2,
			      (rotatedImage.height() - croppedHeight)/2,
			      croppedWidth, croppedHeight);

        croppedImage.setFullImageSize(priv->newFullImageSize);
        croppedImage.setArea(QRect(QPoint(0, 0), priv->newFullImageSize));

	return croppedImage;
    }

    QRect area = newArea(image.fullImageSize(), image.area());
    if ((area.width() <= 0) || (area.height() <= 0))
        return QuillImage();

    // Qt:SmoothTransformation causes unwanted border effects,
    // so we use Qt:FastTransformation for border pixels
    QuillImage newImage = image.transformed(transform, Qt::SmoothTransformation);
    QuillImage newImage2 = image.transformed(transform, Qt::FastTransformation);

    QRgb *endp	= (QRgb*)(newImage.bits()+newImage.numBytes());
    QRgb *q	= (QRgb*) newImage2.bits();

    // Apply the actual operation
    for (QRgb *p=(QRgb*)newImage.bits(); p<endp; p++, q++)
	if (qAlpha(*p) < 255) *p = *q;

    QRect areaNotCropped = transform.mapRect(image.area()).
	translated(priv->translateW, priv->translateH);

    if (areaNotCropped != area)
	 newImage = newImage.copy(area.translated(-areaNotCropped.topLeft()));

    newImage.setFullImageSize(priv->newFullImageSize);
    newImage.setArea(area);

    newImage.convertToFormat(QImage::Format_ARGB32); //image.transformed changes format to _Premultiplied
    return newImage;
}

QSize FreeRotate::newFullImageSize(const QSize &fullImageSize) const
{
    ((FreeRotate*)this)->calculateCutoffs(fullImageSize);
    return priv->newFullImageSize;
}

QRect FreeRotate::newArea(const QSize &fullImageSize, const QRect &area) const
{
    ((FreeRotate*)this)->calculateCutoffs(fullImageSize);
    return priv->transform.mapRect(area).
	translated(priv->translateW, priv->translateH).
        intersected(QRect(QPoint(0, 0), priv->newFullImageSize));
}

void FreeRotate::calculateCutoffs(const QSize &fullImageSize)
{
    const float pi = 3.1415926535898;

    if (fullImageSize == priv->fullImageSize)
    	return;

    priv->fullImageSize = fullImageSize;

    priv->transform = QTransform().rotate(priv->angle);

    int h = fullImageSize.height();
    int w = fullImageSize.width();

    float angle = priv->angle * pi / 180.0;

    // adjust the angle so that we are in 1st or 2nd quarter
    while (angle > pi)
	angle -= pi;
    while (angle < 0)
	angle += pi;
    if (angle > pi/2) // Mirror angles > 90 degrees to 1st quarter
	angle = pi - angle;

    // original diagonal:
    float D_0 = sqrt(w*w + h*h);

    float A, B;
    if (w < h) {
	A = h;
	B = w;
    } else {
	A = w;
	B = h;
    }

    float alpha_0 = atan(B/A);	// angle between diagonal and longer side

    // rotation:
    alpha_0 += angle;

    // lenght of the new diagonal:
    float D = B / sin(alpha_0);

    // Preserve ratio: new width and height are shortened the same amount as the diagonal
    float newWidth  = D/D_0 * w;
    float newHeight = D/D_0 * h;

    // add 1 to cropping, to be sure to clip off all extra pixel
    priv->cropW = (w - newWidth) / 2 + 1;
    priv->cropH = (h - newHeight) / 2 + 1;

    // Assure new image size is larger than zero (original w & h assumed to be > 0)
    if (w - 2*priv->cropW <= 0)
	priv->cropW = (w+1)/2-1;
    if (h - 2*priv->cropH <= 0)
	priv->cropH = (h+1)/2-1;

    priv->newFullImageSize = QSize(w - 2*priv->cropW,
				   h - 2*priv->cropH);

    // Compute the amount to be cut from the rotated full image
    QRectF rotatedRect = priv->transform.mapRect(QRectF(QPoint(0, 0), priv->fullImageSize));

    // Amount of cutting (rounding towards zero):
    priv->translateW = (int)(-(rotatedRect.width()  - newWidth )/2 - rotatedRect.left());
    priv->translateH = (int)(-(rotatedRect.height() - newHeight)/2 - rotatedRect.top());

}

bool FreeRotate::setOption(const QString &option, const QVariant &value)
{
    bool bOK = true;

    if (option == QuillImageFilter::Angle) {
        float angle = value.toDouble(&bOK);
	if (bOK) {
            priv->angle = angle;
	    priv->fullImageSize = QSize(0, 0); // In order to redo cutoff calculations
	}
    }
    else
        bOK = false;

    return bOK;
}

QVariant FreeRotate::option(const QString &option) const
{
    if (option == QuillImageFilter::Angle)
        return QVariant(priv->angle);
    else
        return QVariant();
}

const QStringList FreeRotate::supportedOptions() const
{
    return QStringList() << QuillImageFilter::Angle;
}

