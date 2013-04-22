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
#include "ut_scale.h"

ut_scale::ut_scale()
{
}

void ut_scale::initTestCase()
{
}

void ut_scale::cleanupTestCase()
{
}

// Test scale filter

void ut_scale::testScaleFilter()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.scale");

    QVERIFY(filter);

    QVERIFY(filter->supportsOption(QuillImageFilter::SizeAfter));

    // Assuming full image, rescaling to half size

    QVERIFY(filter->setOption(QuillImageFilter::SizeAfter, QSize(4, 1)));

    QCOMPARE(filter->option(QuillImageFilter::SizeAfter),
             QVariant(QSize(4, 1)));

    QImage image = Unittests::generatePaletteImage();
    QCOMPARE(image.size(), QSize(8, 2));

    QImage filteredImage = filter->apply(image);

    QCOMPARE(filteredImage, image.scaled(QSize(4, 1), Qt::IgnoreAspectRatio,
                                         Qt::SmoothTransformation));

    // full image, rescaling to different aspect ratio

    QVERIFY(filter->setOption(QuillImageFilter::SizeAfter, QSize(2, 8)));

    filteredImage = filter->apply(image);

    QCOMPARE(filteredImage, image.scaled(QSize(2, 8), Qt::IgnoreAspectRatio,
                                         Qt::SmoothTransformation));

    delete filter;
}

void ut_scale::testScalePreview()
{
    // No changes must be made to a preview image

    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.scale");

    QVERIFY(filter);

    QVERIFY(filter->setOption(QuillImageFilter::SizeAfter, QSize(4, 1)));

    QuillImage image = Unittests::generatePaletteImage();
    QCOMPARE(image.size(), QSize(8, 2));

    image.setFullImageSize(QSize(16, 4));
    image.setArea(QRect(0, 0, 16, 4));

    QuillImage image2 = filter->apply(image);

    QCOMPARE(image2.size(), QSize(8, 2));
    QCOMPARE(image2.fullImageSize(), QSize(4, 1));
    QCOMPARE(image2.area(), QRect(0, 0, 4, 1));

    QVERIFY(Unittests::compareImage(image, image2));

    delete filter;
}

void ut_scale::testScaleTiled()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.scale");

    QVERIFY(filter->setOption(QuillImageFilter::SizeAfter, QSize(4, 1)));

    QCOMPARE(filter->newFullImageSize(QSize(8, 2)), QSize(4, 1));
    QCOMPARE(filter->newFullImageSize(QSize(4, 4)), QSize(4, 1));

    // Rounded down
    QCOMPARE(filter->newArea(QSize(8, 2), QRect(2, 0, 3, 2)),
             QRect(1, 0, 1, 1));

    QImage image = Unittests::generatePaletteImage();

    QuillImage image1 = image.copy(0, 0, 3, 2);
    image1.setFullImageSize(QSize(8, 2));
    image1.setArea(QRect(0, 0, 3, 2));

    QuillImage resultImage1 = filter->apply(image1);
    QCOMPARE(resultImage1.area(), QRect(0, 0, 1, 1));
    QCOMPARE(resultImage1.fullImageSize(), QSize(4, 1));
    QCOMPARE((QImage)resultImage1, image1.scaled(QSize(1, 1),
                                                 Qt::IgnoreAspectRatio,
                                                 Qt::SmoothTransformation));

    QuillImage image2 = image.copy(3, 0, 3, 2);
    image2.setFullImageSize(QSize(8, 2));
    image2.setArea(QRect(3, 0, 3, 2));

    QuillImage resultImage2 = filter->apply(image2);
    QCOMPARE(resultImage2.area(), QRect(1, 0, 2, 1));
    QCOMPARE(resultImage2.fullImageSize(), QSize(4, 1));
    QCOMPARE((QImage)resultImage2, image2.scaled(QSize(2, 1),
                                                 Qt::IgnoreAspectRatio,
                                                 Qt::SmoothTransformation));

    QuillImage image3 = image.copy(6, 0, 2, 2);
    image3.setFullImageSize(QSize(8, 2));
    image3.setArea(QRect(6, 0, 2, 2));

    QuillImage resultImage3 = filter->apply(image3);
    QCOMPARE(resultImage3.area(), QRect(3, 0, 1, 1));
    QCOMPARE(resultImage3.fullImageSize(), QSize(4, 1));
    QCOMPARE((QImage)resultImage3, image3.scaled(QSize(1, 1),
                                                 Qt::IgnoreAspectRatio,
                                                 Qt::SmoothTransformation));

    delete filter;
}

// Such a strong downscaling that some tiles get resized into 0x0
void ut_scale::testScaleToMinimal()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.scale");

    QVERIFY(filter->setOption(QuillImageFilter::SizeAfter, QSize(2, 1)));

    QImage image = Unittests::generatePaletteImage();

    QuillImage tile1 = image.copy(0, 0, 2, 2);
    tile1.setFullImageSize(QSize(8, 2));
    tile1.setArea(QRect(0, 0, 2, 2));

    QuillImage tile2 = image.copy(2, 0, 2, 2);
    tile2.setFullImageSize(QSize(8, 2));
    tile2.setArea(QRect(2, 0, 2, 2));

    QuillImage tile3 = image.copy(4, 0, 2, 2);
    tile3.setFullImageSize(QSize(8, 2));
    tile3.setArea(QRect(4, 0, 2, 2));

    QuillImage tile4 = image.copy(6, 0, 2, 2);
    tile4.setFullImageSize(QSize(8, 2));
    tile4.setArea(QRect(6, 0, 2, 2));

    QuillImage resultTile1 = filter->apply(tile1);
    QCOMPARE(resultTile1.area(), QRect());
    QCOMPARE(resultTile1.fullImageSize(), QSize(2, 1));
    QCOMPARE(resultTile1, QuillImage());

    QuillImage resultTile2 = filter->apply(tile2);
    QCOMPARE(resultTile2.area(), QRect(0, 0, 1, 1));
    QCOMPARE(resultTile2.fullImageSize(), QSize(2, 1));
    QCOMPARE((QImage)resultTile2, tile2.scaled(QSize(1, 1),
                                               Qt::IgnoreAspectRatio,
                                               Qt::SmoothTransformation));

    QuillImage resultTile3 = filter->apply(tile3);
    QCOMPARE(resultTile3.area(), QRect());
    QCOMPARE(resultTile3.fullImageSize(), QSize(2, 1));
    QCOMPARE(resultTile3, QuillImage());

    QuillImage resultTile4 = filter->apply(tile4);
    QCOMPARE(resultTile4.area(), QRect(1, 0, 1, 1));
    QCOMPARE(resultTile4.fullImageSize(), QSize(2, 1));
    QCOMPARE((QImage)resultTile4, tile4.scaled(QSize(1, 1),
                                               Qt::IgnoreAspectRatio,
                                               Qt::SmoothTransformation));
    delete filter;
}

void ut_scale::testRoleAndDomainName()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.scale");

    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.scale"));
    delete filter;
}

int main ( int argc, char *argv[] ){
    TestApplication app( argc, argv );
    ut_scale test;
    return QTest::qExec( &test, argc, argv );
}
