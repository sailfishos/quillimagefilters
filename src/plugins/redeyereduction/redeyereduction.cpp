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

#include <QImage>

#include "quillimagefilter.h"
#include "redeyereduction.h"
#include "math.h"


class RedEyeReductionPrivate
{
public:
    //for red eye selection area
    QVariantList selectionArea;
    QVariantList center;
    QVariantList radius;
};

RedEyeReduction::RedEyeReduction()
{
    priv = new RedEyeReductionPrivate;
}

RedEyeReduction::~RedEyeReduction()
{
    delete priv;
}

QuillImage RedEyeReduction::apply(const QuillImage &image) const
{
    QuillImage newImage(image);

    QVariant selectionArea;

    foreach(selectionArea, priv->selectionArea)
    {
	QPolygon p = selectionArea.value<QPolygon>();
	QRect rect = p.boundingRect() & image.area();
	// If the current tile is not affected by the algorithm
	if (rect == QRect()){
	    return image;
	}
	//we consider the cropping case by substracting the origin
	fullImageToFractionRect(image, rect, rect);

	int xMin = rect.left(), xMax = rect.right(),
	yMin = rect.top(), yMax = rect.bottom();
	// Apply the reduction (our own algorithm, based on the red-eye rule
	// in RedEyeDetection class.)

	for (int y=yMin; y<=yMax; y++) {
	    for (int x=xMin; x<=xMax; x++) {
		//we try to convert the red eye points to original file
		QPoint tmp;
		fractionToFullImageCoordinates(image, QPoint(x, y), tmp);
		if (p.containsPoint(tmp, Qt::WindingFill))
		{
		    QRgb color = newImage.pixel(QPoint(x, y));
		    int limit = (qGreen(color) + qBlue(color)) * 2 / 3;
		    if (qRed(color) > limit)
		    {
			newImage.setPixel(QPoint(x, y),
					  qRgb(limit, qGreen(color), qBlue(color)));
		    }
		}
	    }
	}
    }

    return newImage;
}

bool RedEyeReduction::setOption(const QString &option, const QVariant &value)
{
    bool bOK = false;

    if (option == QuillImageFilter::SelectionArea) {
	priv->selectionArea.clear();
        if (value.toList().count() > 0)
            priv->selectionArea.append(value.toList());
        else
            priv->selectionArea.append(value);
        bOK = true;
    }
    else if (option == QuillImageFilter::Center) {
        priv->center.clear();
        if (!value.isNull()) {
            if (value.toList().count() > 0)
                priv->center.append(value.toList());
            else
                priv->center.append(value);
        }
        bOK = true;
        generatePolygon();
    }
    else if (option == QuillImageFilter::Radius) {
        priv->radius.clear();
        if (!value.isNull()) {
            if (value.toList().count() > 0)
                priv->radius.append(value.toList());
            else
                priv->radius.append(value);
        }
        bOK = true;
        generatePolygon();
    }

    return bOK;
}

QVariant RedEyeReduction::option(const QString &option) const
{
    if (option == QuillImageFilter::SelectionArea) {
        if (priv->selectionArea.count() == 1) {
            QPolygon p = priv->selectionArea.first().value<QPolygon>();
            return QVariant(p);
        } else {
            return priv->selectionArea;
        }
    }
    else if (option == QuillImageFilter::Center)
        return QVariant(priv->center);
    else if (option == QuillImageFilter::Radius)
        return QVariant(priv->radius);
    else
        return QVariant();
}

const QStringList RedEyeReduction::supportedOptions() const
{
    return QStringList() << QuillImageFilter::SelectionArea
                         << QuillImageFilter::Center
                         << QuillImageFilter::Radius;
}

void RedEyeReduction::generatePolygon() const
{

    if ( (priv->radius.count() < 1) || (priv->center.count() < 1) )
        return;

    priv->selectionArea.clear();

    for (int i = 0; i < (priv->radius.count() < priv->center.count() ? priv->radius.count() : priv->center.count()) ; i++) {

        if ( (priv->radius[i].toInt() < 1) || (priv->center[i].toPoint().x() < 0) || (priv->center[i].toPoint().y() < 0) )
            continue;

        int x, y;
        QList<QPoint> pointList; // Stores one quadrant of sphere points

        int radius = priv->radius[i].toInt();
        int prev_y = -1; // First y is always positive (radius > 0, x0 = 0)
        for (x = 0; x <= priv->radius[i].toInt(); x++) {
            y = (int) (sqrt(radius*radius - x*x) + .5);
            if (y != prev_y) { // Avoid unnecessary horizontal points
                pointList.append(QPoint(x, y));
                prev_y = y;
            }
        }

        x = priv->center[i].toPoint().x();
        y = priv->center[i].toPoint().y();

        // set points to each quadrant separately, avoiding duplicates
        QList<QPoint>::iterator point, lastpoint;
        QPolygon p;
        p.reserve( 4 * pointList.count() );

        lastpoint = pointList.end();
        lastpoint--;

        for ( point = pointList.begin();
        point != lastpoint;
        point++)  {
            p << QPoint(x + point->x(), y + point->y());
        }

        for (;
             point != pointList.begin();
             point--) {
            p << QPoint(x + point->x(), y - point->y());
        }

        for (;
             point != lastpoint;
             point++) {
            p << QPoint(x - point->x(), y - point->y());
        }

        for (;
             point != pointList.begin();
             point--) {
            p << QPoint(x - point->x(), y + point->y());
        }
        priv->selectionArea.append(p);

    }
}


void RedEyeReduction::getCroppingAndScaling(const QuillImage &image, float &scale, int &cropTop, int &cropLeft) const
{
    float scaleX = image.size().width()  * 1.0 / image.area().width();
    float scaleY = image.size().height() * 1.0 / image.area().height();
    cropLeft = 0;
    cropTop = 0;

    if ( abs(image.size().width() - ( image.size().height()*image.area().width() + image.area().height()/2 ) / image.area().height()) > 2 ) {
	// Image aspect ratio has changed (with more than 2 pixels).
	// We assume that it hasn't happened because of scaling,
	// but a part of the image has been clipped off
	if (scaleX < scaleY) {
	    // Part from left and right has been cropped
	    // Cropped width in original image:
	    float newWidth = image.area().height() *
			     ((float) image.size().width() / image.size().height()) + .5;
	    // Amount of cropping
	    float tmp = (image.area().width() - newWidth + .5) / 2;
	    cropLeft = (int)(tmp + .5);
	    scale = scaleY;
	} else {
	    // Part from top and bottom has been cropped
	    // Cropped height in original image:
	    float newHeight= image.area().width() *
			     ((float) image.size().height() / image.size().width()) + .5;
	    // Amount of cropping
	    float tmp  = (image.area().height() - newHeight + .5) / 2;
	    cropTop = (int)(tmp + .5);
	    scale = scaleX;
	}
    } else {
	// If image aspect ratio has not changed, then scaleX~scaleY
	scale = (scaleX + scaleY) / 2.0;
    }
}

void RedEyeReduction::fractionToFullImageCoordinates (const QuillImage &image,
						      const QPoint &frac,
						      QPoint &full) const
{
    int cropLeft = 0, cropTop = 0;
    float scale = 1.0;

    getCroppingAndScaling(image, scale, cropTop, cropLeft);

    full.setX( ( 1.0*frac.x() / scale ) + image.area().x() + cropLeft + .5);
    full.setY( ( 1.0*frac.y() / scale ) + image.area().y() + cropTop  + .5);
}

void RedEyeReduction::fractionToFullImageRect (const QuillImage &image,
					       const QRect &frac,
					       QRect &full) const
{
    QPoint tmp;
    fractionToFullImageCoordinates(image, frac.bottomLeft(), tmp);
    full.setBottomLeft(tmp);
    fractionToFullImageCoordinates(image, frac.topRight(), tmp);
    full.setTopRight(tmp);
}

void RedEyeReduction::fullImageToFractionCoordinates (const QuillImage &image,
						      const QPoint &full,
						      QPoint &frac) const
{
    int cropLeft = 0, cropTop = 0;
    float scale = 1.0;

    getCroppingAndScaling(image, scale, cropTop, cropLeft);

    frac.setX( (1.0*full.x() - image.area().x() - cropLeft) * scale + .5);
    frac.setY( (1.0*full.y() - image.area().y() - cropTop ) * scale + .5);
}

void RedEyeReduction::fullImageToFractionRect (const QuillImage &image,
					       const QRect &full,
					       QRect &frac) const
{
    QPoint tmp;
    fullImageToFractionCoordinates(image, full.bottomLeft(), tmp);
    frac.setBottomLeft(tmp);
    fullImageToFractionCoordinates(image, full.topRight(), tmp);
    frac.setTopRight(tmp);
}
