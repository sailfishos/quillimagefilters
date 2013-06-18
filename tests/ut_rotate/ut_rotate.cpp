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

#include <QtTest/QtTest>
#include <QImage>
#include <QTransform>

#include <QuillImageFilter>
#include <QuillImageFilterFactory>
#include "unittests.h"
#include "ut_rotate.h"

ut_rotate::ut_rotate()
{
}

void ut_rotate::initTestCase()
{
}

void ut_rotate::cleanupTestCase()
{
}

// Test rotate filter

void ut_rotate::testRotateFilter()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.rotate");

    QVERIFY(filter);

    QVERIFY(filter->supportsOption(QuillImageFilter::Angle));
    QVERIFY(filter->setOption(QuillImageFilter::Angle, QVariant(90)));

    bool bOK = true;
    QCOMPARE(filter->option(QuillImageFilter::Angle).toDouble(&bOK), 90.0);
    QVERIFY(bOK);

    QuillImage image = Unittests::generatePaletteImage();
    QCOMPARE(image.size(), QSize(8, 2));

    QuillImage filteredImage = filter->apply(image);

    QCOMPARE(filteredImage.size(), QSize(2, 8));

    // Get reference image with QImage::transformed()
    QuillImage compareToQImage = image.transformed(QTransform().rotate(90));
    QCOMPARE(filteredImage, compareToQImage);

    // Four rotates should produce the original image
    QCOMPARE(filter->apply(filter->apply(filter->apply(filteredImage))),
        image);

    delete filter;
}

void ut_rotate::testQuillImage()
{
    QuillImage image(Unittests::generatePaletteImage());
    image.setFullImageSize(QSize(16, 4));
    image.setArea(QRect(0, 0, 8, 2));

    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.rotate");
    QVERIFY(filter);

    // Rotate 90 degrees cw
    QuillImage image2 = filter->apply(image);

    QCOMPARE((QImage)image2, image.transformed(QTransform().rotate(90)));
    QCOMPARE(image2.fullImageSize(), QSize(4, 16));
    QCOMPARE(image2.area(), QRect(2, 0, 2, 8));

    // Rotate 180 degrees cw
    filter->setOption(QuillImageFilter::Angle, 180);
    QuillImage image3 = filter->apply(image2);

    QCOMPARE((QImage)image3, image.transformed(QTransform().rotate(-90)));
    QCOMPARE(image3.fullImageSize(), QSize(4, 16));
    QCOMPARE(image3.area(), QRect(0, 8, 2, 8));

    // Rotate 90 degrees ccw
    filter->setOption(QuillImageFilter::Angle, -90);
    QuillImage image4 = filter->apply(image3);

    QCOMPARE((QImage)image4, image.transformed(QTransform().rotate(180)));
    QCOMPARE(image4.fullImageSize(), QSize(16, 4));
    QCOMPARE(image4.area(), QRect(8, 2, 8, 2));

    delete filter;
}

int main ( int argc, char *argv[] ){
    TestApplication app( argc, argv );
    ut_rotate test;
    return QTest::qExec( &test, argc, argv );

}
