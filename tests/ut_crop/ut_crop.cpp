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
#include "ut_crop.h"


ut_crop::ut_crop()
{
}

void ut_crop::initTestCase()
{
}

void ut_crop::cleanupTestCase()
{
}

// Test crop filter

void ut_crop::testFullImage()
{
    QuillImageFilter *filter = //new QuillImageFilter("org.maemo.crop");
        QuillImageFilterFactory::createImageFilter("org.maemo.crop");

    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.crop"));

    QVERIFY(filter->supportsOption(QuillImageFilter::CropRectangle));

    // Full image cropped from right into 6x2

    QVERIFY(filter->setOption(QuillImageFilter::CropRectangle,
                              QRect(QPoint(2, 0), QPoint(7, 1))));

    QCOMPARE(filter->option(QuillImageFilter::CropRectangle),
             QVariant(QRect(QPoint(2, 0), QPoint(7, 1))));

    QImage image = Unittests::generatePaletteImage();
    QCOMPARE(image.size(), QSize(8, 2));

    QImage filteredImage = filter->apply(image);

    QCOMPARE(filteredImage, image.copy(2, 0, 6, 2));

    delete filter;
}

// Test crop for preview

void ut_crop::testPreview()
{
    // full is 8x2, preview is 4x1, apply crop to preview

    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.crop");

    QVERIFY(filter->setOption(QuillImageFilter::CropRectangle,
                              QRect(QPoint(2, 0), QPoint(7, 1))));

    QuillImage image(Unittests::generatePaletteImage().scaled(QSize(4,1)));

    image.setFullImageSize(QSize(8, 2));
    image.setArea(QRect(0, 0, 8, 2));

    QuillImage filteredImage = filter->apply(image);

    QCOMPARE((QImage)filteredImage, image.copy(1, 0, 3, 1));

    delete filter;
}

void ut_crop::testQuillImageFragment()
{
    QuillImage image(Unittests::generatePaletteImage());
    image.setFullImageSize(QSize(16, 4));
    image.setArea(QRect(4, 1, 8, 2));

    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.crop");
    QVERIFY(filter);

    // crop 6 px from the left
    filter->setOption(QuillImageFilter::CropRectangle,
                      QRect(6, 0, 10, 4));
    QuillImage image2 = filter->apply(image);

    QCOMPARE((QImage)image2, image.copy(QRect(2, 0, 6, 2)));
    QCOMPARE(image2.fullImageSize(), QSize(10, 4));
    QCOMPARE(image2.area(), QRect(0, 1, 6, 2));

    // crop 6 px from the right
    filter->setOption(QuillImageFilter::CropRectangle,
                      QRect(0, 0, 4, 4));
    QuillImage image3 = filter->apply(image2);

    QCOMPARE((QImage)image3, image.copy(QRect(2, 0, 4, 2)));
    QCOMPARE(image3.fullImageSize(), QSize(4, 4));
    QCOMPARE(image3.area(), QRect(0, 1, 4, 2));

    // crop to bottom half
    filter->setOption(QuillImageFilter::CropRectangle,
                      QRect(0, 2, 4, 2));
    QuillImage image4 = filter->apply(image3);

    QCOMPARE((QImage)image4, image.copy(QRect(2, 1, 4, 1)));
    QCOMPARE(image4.fullImageSize(), QSize(4, 2));
    QCOMPARE(image4.area(), QRect(0, 0, 4, 1));

    delete filter;
}

void ut_crop::testQuillImagePreview()
{
    QuillImage image(Unittests::generatePaletteImage());
    image.setFullImageSize(QSize(16, 4));
    image.setArea(QRect(0, 0, 16, 4));

    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.crop");
    QVERIFY(filter);

    // crop 6/3 px from the left
    filter->setOption(QuillImageFilter::CropRectangle,
                      QRect(6, 0, 10, 4));
    QuillImage image2 = filter->apply(image);

    QVERIFY(Unittests::compareImage(image2, image.copy(QRect(3, 0, 5, 2))));
    QCOMPARE(image2.fullImageSize(), QSize(10, 4));
    QCOMPARE(image2.area(), QRect(0, 0, 10, 4));

    delete filter;
}

int main ( int argc, char *argv[] ){
    TestApplication app( argc, argv );
    ut_crop test;
    return QTest::qExec( &test, argc, argv );

}
