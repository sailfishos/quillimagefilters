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
  \class RedEyeDetection

  \brief Filter generator to detect red eyes to be removed by RedEyeReduction.

Domain based name: org.maemo.red-eye-detection

Options supported: SelectionArea (polygon approximating the eye area
in full-image coordinates).

Default behavior: none.
*/

#ifndef __QUILL_RED_EYE_DETECTION_H__
#define __QUILL_RED_EYE_DETECTION_H__

#include <QColor>
#include <QPoint>
#include <QList>
#include <QSet>
#include "quillimagefiltergeneratorimplementation.h"
class RedEyeDetectionPrivate;
class QVariant;
class QPoint;
class QRect;
class QPolygon;

class RedEyeDetection : public QuillImageFilterGeneratorImplementation
{
    friend class TestRedEyeReduction;

    /*!
      Detects a red eye, generates a reduction filter
    */

public:
    RedEyeDetection();
    ~RedEyeDetection();

    QuillImageFilter *generate(const QuillImage &image) const;
    bool setOption(const QString &option, const QVariant &value);
    bool isUsedOnPreview() const;
    const QString name() const {return QuillImageFilter::Name_RedEyeDetection; }
    const QStringList supportedOptions() const;
    QVariant option(const QString &option) const;

private:
    RedEyeDetectionPrivate *priv;

    QRect getStartingRectangle(const QSize &imageSize,
                               const QSize &fullImageSize) const;
    QPoint locateStartingPoint(const QImage &image,
                               const QRect &rect) const;
    QSet<QPoint> expandRedEye(const QImage &image,
                              const QPoint &center,
                              const QRect &area) const;
    QPolygon generatePolygon(const QRect &area,
                             const QSet<QPoint> &points) const;
    QPoint scalePoint(const QPoint &point,
                      const QSize &scaleBefore,
                      const QSize &scaleAfter) const;
    QPolygon scalePolygon(const QPolygon &polygon,
                          const QSize &scaleBefore,
                          const QSize &scaleAfter) const;

    void setThreshold(int pass) const;
    bool isRedEyePixel(QRgb color) const;
};

/* To be able to store QPoints in a QHash */

inline uint qHash(const QPoint &point)
{
    return qHash(point.x() * 4096 + point.y());
}

#endif //__QUILL_AUTO_CONTRAST_H__
