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

#include "redeyedetection.h"
#include <QImage>
#include <QSet>
#include <QList>
#include <QQueue>
#include <QPolygon>
#include <QRect>

#include "quillimagefilterfactory.h"

class RedEyeDetectionPrivate
{
public:
    /* permanent data */
    QPoint center;
    int eyeRadius;
    int tapErrorTolerance;
    /* non-permanent data */
    int redChannel;
    int redRatio, greenRatio, blueRatio;

    static const int redChannelBase = 50;
    static const int redRatioBase = 40;
    static const int greenRatioBase = 31;
    static const int blueRatioBase = 36;
};

RedEyeDetection::RedEyeDetection()
{
    priv = new RedEyeDetectionPrivate;
    priv->center = QPoint();
    priv->eyeRadius = 0;
    priv->tapErrorTolerance = 0;
}

RedEyeDetection::~RedEyeDetection()
{
    delete priv;
}

/*!
  The red eye detection works this way:
  * Search all points which are less than a given distance from the
  * starting point. Of these points, select the one which has
  * the largest red component relative to other color components.
  *
  * From this starting point, apply a flood fill selection algorithm,
  * selecting those points which are considered "red" enough.
  *
  * If the flood fill overflows the selected area, progressively
  * tighten the definition of "red" until the flooded area stays.
  * reasonable.
  *
  * Soften the edges of the reasonable area if pixels near the edge
  * are near the definition of "red".
  *
  * Construct a polygon from the results.
  *
  * Warning: As other generators, this algorithm is supposed to run in
  * synchronous time. If the given radius is too big, the algorithm
  * may become very slow!
  */

QuillImageFilter *RedEyeDetection::generate(const QuillImage &image) const
{
    // No reference image given; no operations possible.
    if (image == QuillImage())
        return 0;

    // Scale the search rectangle from full-image coordinates to current
    // image.

    QRect rect = getStartingRectangle(image.size(), image.fullImageSize());

    // Detect best possible starting point

    QPoint center = locateStartingPoint(image, rect);

    // No possible red-eye pixels found - aborting (this should
    // generally not happen unless you select the blue sky or
    // something)
    if (center == QPoint())
        return 0;

    // Generate legal area around center
    int radius = priv->eyeRadius * qMin(image.size().width(), image.size().height()) /
	qMin(image.fullImageSize().width(), image.fullImageSize().height());
    QRect area = QRect(center.x()-radius, center.y()-radius,
		       radius*2+1, radius*2+1);

    area &= image.rect();

    // Flood fill around selected center
    // If overflows, tighten the criteria and try again.

    QSet<QPoint> redEye;

    for (int phase = 0; phase < 5; phase++)
    {
        setThreshold(phase);
        redEye = expandRedEye(image, center, area);
        if (!redEye.isEmpty())
            break;
    }

    // Still overflows - abandon

    if (redEye.isEmpty())
        return 0;

    QPolygon polygon = generatePolygon(area, redEye);

    // For some reason, polygon was empty (this should never happen)

    if (polygon == QPolygon())
        return 0;

    // If this was a preview, scale the plugin to big-picture coordinates

    if ((image.fullImageSize() != QSize()) && image.fullImageSize() != image.size())
        polygon = scalePolygon(polygon, image.size(), image.fullImageSize());

    // All is well.

    QuillImageFilter *filter =
	QuillImageFilterFactory::createImageFilter(QuillImageFilter::Name_RedEyeReduction);

    if (!filter)
        return 0;

    filter->setOption(QuillImageFilter::SelectionArea, polygon);

    return filter;
}

QRect RedEyeDetection::getStartingRectangle(const QSize &imageSize,
                                            const QSize &fullImageSize) const
{
    int tolerance = (priv->tapErrorTolerance <= 0 ? priv->eyeRadius : priv->tapErrorTolerance);
    //if tapErrorTolerance is unset, we user eyeRadius
    QRect rect = QRect(scalePoint(QPoint(priv->center.x() - tolerance,
					 priv->center.y() - tolerance),
                                  fullImageSize, imageSize),
		       scalePoint(QPoint(priv->center.x() + tolerance,
					 priv->center.y() + tolerance),
                                  fullImageSize, imageSize));

    rect &= QRect(QPoint(0, 0), imageSize);

    return rect;
}

QPoint RedEyeDetection::locateStartingPoint(const QImage &image,
					    const QRect &rect) const
{
    // Find optimal starting point (one with highest red value
    // relative to green and blue)

    const int coeflen = 6;
    const int coefs[13] = {28, 39, 48, 55, 60, 63, 64,
			   63, 60, 55, 48, 39, 28}; // 8*8 - {0,1,2...}^2

    QPoint centerMax = QPoint();
    int redMax = 0, redRatio = 0;

    // The center point is set in full-image coordinates

    setThreshold(0);

    int x_c = rect.center().x();
    int y_c = rect.center().y();
    int d_max = qMax(rect.width()/2, rect.height()/2);

    for (int y=rect.top(); y<=rect.bottom(); y++)
    {
	int y_coef = coefs[(coeflen*(y-y_c))/d_max + coeflen];
	for (int x=rect.left(); x <= rect.right(); x++)
	{
	    QRgb rgb = image.pixel(QPoint(x, y));

	    if (isRedEyePixel(rgb))
	    {
		redRatio = qRed(rgb) * 255 /
			   (qRed(rgb) + qGreen(rgb) + qBlue(rgb));

		int x_coef = coefs[(coeflen*(x-x_c))/d_max + coeflen];
		redRatio *= x_coef + y_coef;

		if (redRatio > redMax) {
		    redMax = redRatio;
		    centerMax = QPoint(x, y);
		}
	    }
	}
    }

    return centerMax;
}


QSet<QPoint> RedEyeDetection::expandRedEye(const QImage &image,
					   const QPoint &center,
					   const QRect &area) const
{
    QSet<QPoint> visited, included;
    QQueue<QPoint> active;

    visited << center;
    included << center;

    active.enqueue(center);

    QPoint neighbors[8];

    while (!active.isEmpty()) {
	QPoint point = active.dequeue();

	neighbors[0] = point+QPoint(-1, -1);
	neighbors[1] = point+QPoint(0, -1);
	neighbors[2] = point+QPoint(+1, -1);
	neighbors[3] = point+QPoint(-1, 0);
	neighbors[4] = point+QPoint(+1, 0);
	neighbors[5] = point+QPoint(-1, +1);
	neighbors[6] = point+QPoint(0, +1);
	neighbors[7] = point+QPoint(+1, +1);

	for (int i=0; i<8; i++)
	{
	    if (!visited.contains(neighbors[i]))
	    {
		visited << neighbors[i];

		// Mark only red neighbors as active and included
		if (isRedEyePixel(image.pixel(neighbors[i]))) {
		    included << neighbors[i];
		    active.enqueue(neighbors[i]);

		    if (!area.contains(neighbors[i], true))
		    {
			included.clear();
			active.clear();
			break;
		    }

		}
	    }
	}
    }
    return included;
}

// Generate simple polygon using given points

QPolygon RedEyeDetection::generatePolygon(const QRect &area,
                                          const QSet<QPoint> &points) const
{
    int yMin = area.top(), yMax = area.bottom();

    int xMin[area.height()+1],
        xMax[area.height()+1];

    for (int y=0; y<area.height(); y++)
    {
        xMax[y] = -1;
        xMin[y] = 1000000000;
    }

    // Iterate over points, get the minimum values of x for all y

    for (QSet<QPoint>::const_iterator point = points.begin();
         point != points.end();
         point++)
    {
        int y = point->y(), x = point->x();

        if ((y >= yMin+1) && (y <= yMax-1))
        {
            if ((x < xMin[y-yMin]))
                xMin[y-yMin] = x - 2;
            if ((x-1 < xMin[y-yMin-1]))
                xMin[y-yMin-1] = x - 1;
            if ((x+1 < xMin[y-yMin+1]))
            xMin[y-yMin+1] = x - 1;
            if ((x > xMax[y-yMin]))
                xMax[y-yMin] = x + 3;
            if ((x > xMax[y-yMin-1]))
                xMax[y-yMin-1] = x + 2;
            if ((x > xMax[y-yMin+1]))
            xMax[y-yMin+1] = x + 2;
        }
    }

    QPolygon polygon;

    for (int y=yMin; y<=yMax; y++)
        if (xMin[y-yMin] != 1000000000)
            polygon << QPoint(xMin[y-yMin], y);

    for (int y=yMax; y>=yMin; y--)
        if (xMax[y-yMin] != -1)
            polygon << QPoint(xMax[y-yMin], y);

    return polygon;
}

/* Used to create thresholds for an appropriate pass */

void RedEyeDetection::setThreshold(int pass) const
{
    priv->redChannel = priv->redChannelBase + pass;
    priv->redRatio = priv->redRatioBase + pass;
    priv->greenRatio = priv->greenRatioBase - pass;
    priv->blueRatio = priv->blueRatioBase - pass;
}

/* Strong condition - for flood filling */

bool RedEyeDetection::isRedEyePixel(QRgb color) const
{
    int base = qRed(color) + qGreen(color) + qBlue(color);

    return ((qRed(color) > priv->redChannel) &&
	    (qRed(color)   * 100 / base > priv->redRatio  ) &&
            (qGreen(color) * 100 / base < priv->greenRatio) &&
	    (qBlue(color)  * 100 / base < priv->blueRatio ));
}

QPoint RedEyeDetection::scalePoint(const QPoint &point,
                                   const QSize &scaleBefore,
                                   const QSize &scaleAfter) const
{
    return QPoint(point.x() * scaleAfter.width() / scaleBefore.width(),
                  point.y() * scaleAfter.height() / scaleBefore.height());
}

QPolygon RedEyeDetection::scalePolygon(const QPolygon &polygon,
                                       const QSize &scaleBefore,
                                       const QSize &scaleAfter) const
{
    QPolygon newPolygon(polygon);

    for (int i=0; i<newPolygon.size(); i++)
        newPolygon.setPoint(i, scalePoint(newPolygon.point(i),
                                          scaleBefore, scaleAfter));
    return newPolygon;
}

bool RedEyeDetection::setOption(const QString &option, const QVariant &value)
{
    bool bOk = true;

    if (option == QuillImageFilter::Center)
        priv->center = value.toPoint();
    else if (option == QuillImageFilter::Radius)
	priv->eyeRadius = value.toInt(&bOk);
    else if (option == QuillImageFilter::Tolerance)
	priv->tapErrorTolerance = value.toInt(&bOk);
    else
        bOk = false;

    return bOk;
}

const QStringList RedEyeDetection::supportedOptions() const
{
    return QStringList() << QuillImageFilter::Center
			 << QuillImageFilter::Radius
			 << QuillImageFilter::Tolerance;
}

QVariant RedEyeDetection::option(const QString &option) const
{
    if (option == QuillImageFilter::Center)
        return QVariant(priv->center);
    else if (option == QuillImageFilter::Radius)
	return QVariant(priv->eyeRadius);
    else if (option == QuillImageFilter::Tolerance)
	return QVariant(priv->tapErrorTolerance);
    else
        return QVariant();
}

bool RedEyeDetection::isUsedOnPreview() const
{
    return false;
}

