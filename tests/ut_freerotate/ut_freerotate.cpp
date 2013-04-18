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
#include <QuillImageFilterGenerator>
#include "unittests.h"
#include "ut_freerotate.h"

ut_freerotate::ut_freerotate()
{
}

void ut_freerotate::initTestCase()
{
}

void ut_freerotate::cleanupTestCase()
{
}

void ut_freerotate::testBasics()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");

    QVERIFY(filter);
    QVERIFY(filter->supportsOption(QuillImageFilter::Angle));

    filter->setOption(QuillImageFilter::Angle, QVariant(10));
    QCOMPARE(filter->option(QuillImageFilter::Angle), QVariant(10));

    delete filter;
}

void ut_freerotate::testNewFullImageSize()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");
    QVERIFY(filter);

    filter->setOption(QuillImageFilter::Angle, QVariant(10));
    QCOMPARE(filter->newFullImageSize(QSize(375, 500)),
	     QSize(307, 410));

    filter->setOption(QuillImageFilter::Angle, QVariant(-10));
    QCOMPARE(filter->newFullImageSize(QSize(375, 500)),
	     QSize(307, 410));

    delete filter;
}

void ut_freerotate::testNewArea()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");
    QVERIFY(filter);

    filter->setOption(QuillImageFilter::Angle, QVariant(10));
    QCOMPARE(filter->newArea(QSize(375, 500), QRect(0, 0, 100, 100)),
	     QRect(0, 0, 110, 43));
    QCOMPARE(filter->newArea(QSize(375, 500), QRect(100, 100, 100, 100)),
	     QRect(76, 43, 116, 116));

    delete filter;
}

void ut_freerotate::testApply()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");
    QVERIFY(filter);

    QuillImage image(QImage(QSize(5, 5), QImage::Format_ARGB32));
    image.fill(qRgba(255, 255, 255, 255));

    filter->setOption(QuillImageFilter::Angle, QVariant(5));

    QCOMPARE(filter->newFullImageSize(QSize(5, 5)),
	     QSize(3, 3));

    QuillImage rotatedImage = filter->apply(image);

    QCOMPARE(rotatedImage.size(), QSize(3, 3));

    delete filter;
}

void ut_freerotate::testSizeDoesntGoToZeroHorizontal()
{
    QImage image("/usr/share/quillimagefilter-tests/images/redeye.jpg");
    QVERIFY(!image.isNull());

    QuillImageFilter *filter =
	QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");
    QVERIFY(filter);

    QuillImage rotatedImage = image;

    while ((image.width() > 1)&&(image.height() > 1))
    {
	image = rotatedImage;
	filter->setOption(QuillImageFilter::Angle, QVariant(45));
	rotatedImage = filter->apply(image);
	QVERIFY(rotatedImage.width()  > 0);
	QVERIFY(rotatedImage.height() > 0);
    }

    delete filter;
}

void ut_freerotate::testSizeDoesntGoToZeroVertical()
{
    QImage image("/usr/share/quillimagefilter-tests/images/redeye.jpg");
    QVERIFY(!image.isNull());

    QuillImageFilter *filter0 =
	QuillImageFilterFactory::createImageFilter("org.maemo.rotate");
    QVERIFY(filter0);
    filter0->setOption(QuillImageFilter::Angle, QVariant(90));
    image = filter0->apply(image);

    QuillImageFilter *filter =
	QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");
    QVERIFY(filter);

    QuillImage rotatedImage = image;

    while ((image.width() > 1)&&(image.height() > 1))
    {
	image = rotatedImage;
	rotatedImage = filter->apply(image);
	filter->setOption(QuillImageFilter::Angle, QVariant(45));
	QVERIFY(rotatedImage.width()  > 0);
	QVERIFY(rotatedImage.height() > 0);
    }

    delete filter;
}

void ut_freerotate::testPreserveRatioHorizontal()
{
    QImage image("/usr/share/quillimagefilter-tests/images/redeye.jpg");
    QVERIFY(!image.isNull());

    QuillImageFilter *filter =
	QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");
    QVERIFY(filter);

    for (int angle = -90; angle <= 90; angle += 10)
    {
	filter->setOption(QuillImageFilter::Angle, QVariant(angle));
	QuillImage rotatedImage = filter->apply(image);
	QVERIFY(fabs( ((float)rotatedImage.width()/image.width()) -
		      ((float)rotatedImage.height()/image.height()) )
		< 0.01);
    }
    delete filter;
}

void ut_freerotate::testPreserveRatioVertical()
{
    QImage image("/usr/share/quillimagefilter-tests/images/redeye.jpg");
    QVERIFY(!image.isNull());

    QuillImageFilter *filter0 =
	QuillImageFilterFactory::createImageFilter("org.maemo.rotate");
    QVERIFY(filter0);
    filter0->setOption(QuillImageFilter::Angle, QVariant(90));
    image = filter0->apply(image);

    QuillImageFilter *filter =
	QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");
    QVERIFY(filter);

    for (int angle = -90; angle <= 90; angle += 10)
    {
	filter->setOption(QuillImageFilter::Angle, QVariant(angle));
	QuillImage rotatedImage = filter->apply(image);
	QVERIFY(fabs( ((float)rotatedImage.width()/image.width()) -
		      ((float)rotatedImage.height()/image.height()) )
		< 0.01);
    }

    delete filter;
}

void ut_freerotate::testZeroImageSize()
{
    QImage qimage(1, 1, QImage::Format_RGB32);
    QVERIFY(!qimage.isNull());
    QuillImage image(qimage, QSize(0, 0));
    QVERIFY(!image.isNull());

    QuillImageFilter *filter =
	QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");
    QVERIFY(filter);

    filter->setOption(QuillImageFilter::Angle, QVariant(90));
    QuillImage rotatedImage = filter->apply(image);
    QCOMPARE(rotatedImage.width(), 0);
    QCOMPARE(rotatedImage.height(), 0);

    delete filter;
}

void ut_freerotate::testWithBrightnessAndContrast()
{
    QuillImage targetImage;
    QuillImage editedTargetImage;
    QuillImage intermediateImage;
    QuillImage sourceImage(Unittests::generatePaletteImageWithAlpha(64, 64));
    sourceImage.setFullImageSize(QSize(64, 64));
    sourceImage.setArea(QRect(24, 24, 8, 8));

    int rotationAngle = 10;
    const int absBrightnessChange = 10;
    const int absContrastChange = 10;

    // Create a dark source image
    {
	QuillImageFilter *filter =
	    QuillImageFilterFactory::createImageFilter("com.meego.composite.brightness.contrast");
	QVERIFY(filter);

	QVERIFY(filter->setOption(QuillImageFilter::Brightness, QVariant(-absBrightnessChange)));
	QVERIFY(filter->setOption(QuillImageFilter::Contrast, QVariant(-absContrastChange)));

	sourceImage = filter->apply(sourceImage);

	delete filter;
    }

    //***************************
    // Create an image by rotating there and back
    {
	QuillImageFilter *filter =
		QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");
	QVERIFY(filter);

	filter->setOption(QuillImageFilter::Angle, QVariant(rotationAngle));
	intermediateImage = filter->apply(sourceImage);

	delete filter;
    }
    {
	QuillImageFilter *filter =
		QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");
	QVERIFY(filter);

	filter->setOption(QuillImageFilter::Angle, QVariant(-rotationAngle));
	targetImage = filter->apply(intermediateImage);

	delete filter;
    }

    //***************************
    // Create an image by rotating there and back, doing brightness-contrast in between
    {
	QuillImageFilter *filter =
		QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");
	QVERIFY(filter);

	filter->setOption(QuillImageFilter::Angle, QVariant(rotationAngle));
	intermediateImage = filter->apply(sourceImage);

	delete filter;
    }
    // Lighten up
    {
	QuillImageFilter *filter =
	    QuillImageFilterFactory::createImageFilter("com.meego.composite.brightness.contrast");
	QVERIFY(filter);

	QVERIFY(filter->setOption(QuillImageFilter::Brightness, QVariant(+absBrightnessChange)));
	QVERIFY(filter->setOption(QuillImageFilter::Contrast, QVariant(-absContrastChange)));

	intermediateImage = filter->apply(intermediateImage);

	delete filter;
    }
    {
	QuillImageFilter *filter =
		QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");
	QVERIFY(filter);

	filter->setOption(QuillImageFilter::Angle, QVariant(-rotationAngle));
	editedTargetImage  = filter->apply(intermediateImage);

	delete filter;
    }

    // Compare results
    QRgb *endp_source = (QRgb*)(targetImage.bits()+targetImage.numBytes());
    QRgb *p_source = (QRgb*)targetImage.bits();
    QRgb *p_target = (QRgb*)editedTargetImage.bits();
    for (; p_source < endp_source; p_source++, p_target++)
    {
	QCOMPARE(qAlpha(*p_source), qAlpha(*p_target));
    }
}

int main ( int argc, char *argv[] ){
    QCoreApplication app( argc, argv );
    ut_freerotate test;
    return QTest::qExec( &test, argc, argv );

}
