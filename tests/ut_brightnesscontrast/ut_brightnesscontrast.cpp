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
#include <QuillImageFilter>
#include <QuillImageFilterFactory>

#include "unittests.h"
#include "ut_brightnesscontrast.h"
#include "quillimagefilterimplementation.h"

#include <QDebug>

ut_brightnesscontrast::ut_brightnesscontrast()
{
}

void ut_brightnesscontrast::initTestCase()
{
}

void ut_brightnesscontrast::cleanupTestCase()
{
}

// Test brightness component

void ut_brightnesscontrast::testBrightness()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.composite.brightness.contrast");

    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.composite.brightness.contrast"));

    QVERIFY(filter->supportsOption(QuillImageFilter::Brightness));
    QVERIFY(filter->setOption(QuillImageFilter::Brightness, QVariant(25)));

    bool bOK = true;
    QCOMPARE(filter->option(QuillImageFilter::Brightness).toDouble(&bOK), 25.0);
    QVERIFY(bOK);

    QImage image = Unittests::generatePaletteImage();

    QCOMPARE(image.width(), 8);
    QCOMPARE(image.height(), 2);

    QImage filteredImage = filter->apply(image);

    QCOMPARE(filteredImage.width(), 8);
    QCOMPARE(filteredImage.height(), 2);

    // Simple implementation of brightness filter here. 25% = 63.75 ~ 64 units.

    for (int p=0; p<16; p++)
    {
        int rgb = image.pixel(p%8, p/8);
        int red = qRed(rgb) + 64;
        if (red > 255) red = 255;
        int green = qGreen(rgb) + 64;
        if (green > 255) green = 255;
        int blue = qBlue(rgb) + 64;
        if (blue > 255) blue = 255;
        image.setPixel(p%8, p/8, qRgb(red, green, blue));
    }

    QCOMPARE(filteredImage, image);

    delete filter;
}

// Test contrast component

void ut_brightnesscontrast::testContrast()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.composite.brightness.contrast");

    QVERIFY(filter);

    QVERIFY(filter->supportsOption(QuillImageFilter::Contrast));
    QVERIFY(filter->setOption(QuillImageFilter::Contrast, QVariant(50)));

    bool bOK = true;
    QCOMPARE(filter->option(QuillImageFilter::Contrast).toDouble(&bOK), 50.0);
    QVERIFY(bOK);

    QImage image = Unittests::generatePaletteImage();

    QCOMPARE(image.width(), 8);
    QCOMPARE(image.height(), 2);

    QImage filteredImage = filter->apply(image);

    QCOMPARE(filteredImage.width(), 8);
    QCOMPARE(filteredImage.height(), 2);

    // Simple implementation of contrast filter here

    for (int p=0; p<16; p++)
    {
        int rgb = image.pixel(p%8, p/8);
        int red = (int)(127.5 + (qRed(rgb) - 127.5) * 3 / 2);
        if (red > 255) red = 255;
        if (red < 0) red = 0;
        int green = (int)(127.5 + (qGreen(rgb) - 127.5) * 3 / 2);
        if (green > 255) green = 255;
        if (green < 0) green = 0;
        int blue = (int)(127.5 + (qBlue(rgb) - 127.5) * 3 / 2);
        if (blue > 255) blue = 255;
        if (blue < 0) blue = 0;
        image.setPixel(p%8, p/8, qRgb(red, green, blue));
    }

    QCOMPARE(filteredImage, image);

    delete filter;
}

// Test combined brightness & contrast operation

void ut_brightnesscontrast::testCombined()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.composite.brightness.contrast");

    QVERIFY(filter);

    QVERIFY(filter->supportsOption(QuillImageFilter::Brightness));
    QVERIFY(filter->supportsOption(QuillImageFilter::Contrast));
    QVERIFY(filter->setOption(QuillImageFilter::Brightness, QVariant(-50)));
    QVERIFY(filter->setOption(QuillImageFilter::Contrast, QVariant(-50)));

    bool bOK = true;
    QCOMPARE(filter->option(QuillImageFilter::Brightness).toDouble(&bOK), -50.0);
    QVERIFY(bOK);
    QCOMPARE(filter->option(QuillImageFilter::Contrast).toDouble(&bOK), -50.0);
    QVERIFY(bOK);

    QImage image = Unittests::generatePaletteImage();

    QCOMPARE(image.width(), 8);
    QCOMPARE(image.height(), 2);

    QImage filteredImage = filter->apply(image);

    QCOMPARE(filteredImage.width(), 8);
    QCOMPARE(filteredImage.height(), 2);

    // Brightness -50% and contrast -50% should halve all the color values
    // (odd numbers rounded up)

    for (int p=0; p<16; p++)
    {
        int rgb = image.pixel(p%8, p/8);
        int red = (qRed(rgb)+1) / 2;
        int green = (qGreen(rgb)+1) / 2;
        int blue = (qBlue(rgb)+1) / 2;
        image.setPixel(p%8, p/8, qRgb(red, green, blue));

        /*
          printf("%x %x\n", image.pixel(QPoint(p%8, p/8)),
          filteredImage.pixel(QPoint(p%8, p/8)));
        */
	qDebug() << QString("%1").arg(image.pixel(QPoint(p%8, p/8)), 0, 16)
		<< " "
		<< QString("%1").arg(filteredImage.pixel(QPoint(p%8, p/8)), 0, 16);
    }

    QVERIFY(Unittests::compareImage(filteredImage, image));

    delete filter;
}

void ut_brightnesscontrast::testPreserveAlphaChannel()
{
    QuillImage targetImage;
    QImage sourceImage(Unittests::generatePaletteImageWithAlpha(8, 8));

    QuillImageFilter *filter =
	    QuillImageFilterFactory::createImageFilter("org.maemo.composite.brightness.contrast");

    targetImage = filter->apply(sourceImage);

    QRgb *endp_source = (QRgb*)(sourceImage.bits()+sourceImage.numBytes());
    QRgb *p_source = (QRgb*)sourceImage.bits();
    QRgb *p_target = (QRgb*)targetImage.bits();
    for (; p_source < endp_source; p_source++, p_target++)
    {
	QCOMPARE(qAlpha(*p_source), qAlpha(*p_target));
    }

    delete filter;
}

int main ( int argc, char *argv[] ){
    QCoreApplication app( argc, argv );
    ut_brightnesscontrast test;
    return QTest::qExec( &test, argc, argv );

}
