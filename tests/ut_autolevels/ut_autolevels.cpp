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
#include <QuillImageFilterGenerator>

#include "unittests.h"
#include "ut_autolevels.h"

ut_autolevels::ut_autolevels()
{
}

void ut_autolevels::initTestCase()
{
}

void ut_autolevels::cleanupTestCase()
{
}

void ut_autolevels::testGeneratorBasics()
{
    QuillImageFilter *filter =
            QuillImageFilterFactory::createImageFilter("com.meego.auto.levels");

    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.auto.levels"));

    delete filter;
}

void ut_autolevels::testLevelsBasics()
{
    QuillImageFilter *filter =
            QuillImageFilterFactory::createImageFilter("org.maemo.levels");

    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.levels"));
    QVERIFY(filter->supportsOption(QuillImageFilter::Red));
    QVERIFY(filter->supportsOption(QuillImageFilter::RedMax));
    QVERIFY(filter->supportsOption(QuillImageFilter::Green));
    QVERIFY(filter->supportsOption(QuillImageFilter::GreenMax));
    QVERIFY(filter->supportsOption(QuillImageFilter::Blue));
    QVERIFY(filter->supportsOption(QuillImageFilter::BlueMax));

    filter->setOption(QuillImageFilter::Red, QVariant(10));
    filter->setOption(QuillImageFilter::RedMax, QVariant(20));
    filter->setOption(QuillImageFilter::Green, QVariant(30));
    filter->setOption(QuillImageFilter::GreenMax, QVariant(40));
    filter->setOption(QuillImageFilter::Blue, QVariant(50));
    filter->setOption(QuillImageFilter::BlueMax, QVariant(60));

    QCOMPARE(filter->option(QuillImageFilter::Red), QVariant(10));
    QCOMPARE(filter->option(QuillImageFilter::RedMax), QVariant(20));
    QCOMPARE(filter->option(QuillImageFilter::Green), QVariant(30));
    QCOMPARE(filter->option(QuillImageFilter::GreenMax), QVariant(40));
    QCOMPARE(filter->option(QuillImageFilter::Blue), QVariant(50));
    QCOMPARE(filter->option(QuillImageFilter::BlueMax), QVariant(60));

    delete filter;
}

/*!
  Basic auto levels. Test with three pixels.
*/

void ut_autolevels::testGenerator()
{
    QImage image = QImage(QSize(3, 1), QImage::Format_RGB32);

    image.setPixel(QPoint(0, 0), qRgb(0, 64, 127));
    image.setPixel(QPoint(1, 0), qRgb(64, 128, 191));
    image.setPixel(QPoint(2, 0), qRgb(128, 192, 255));

    QuillImageFilter *newFilterGenerator =
            QuillImageFilterFactory::createImageFilter("org.maemo.auto.levels");
    QVERIFY(newFilterGenerator);
    QCOMPARE(newFilterGenerator->name(), QString("com.meego.auto.levels"));

    QuillImageFilterGenerator *filterGenerator =
            dynamic_cast<QuillImageFilterGenerator*>(newFilterGenerator);
    QVERIFY(filterGenerator);
    if(filterGenerator){
        filterGenerator->apply(image);

        QuillImageFilter *filter = filterGenerator->resultingFilter();
        QCOMPARE(filter->name(), QString("com.meego.levels"));

        // Red: 0 to 128
        QCOMPARE(filter->option(QuillImageFilter::Red).toInt(), 0);
        QCOMPARE(filter->option(QuillImageFilter::RedMax).toInt(), 128);
        // Green: 64 to 192
        QCOMPARE(filter->option(QuillImageFilter::Green).toInt(), 64);
        QCOMPARE(filter->option(QuillImageFilter::GreenMax).toInt(), 192);
        // Blue: 127 to 255
        QCOMPARE(filter->option(QuillImageFilter::Blue).toInt(), 127);
        QCOMPARE(filter->option(QuillImageFilter::BlueMax).toInt(), 255);
        image = filter->apply(image);
        // See that the contrast has really been stretched throughout all levels.
        QCOMPARE(image.pixel(QPoint(0, 0)), qRgb(0, 0, 0));
        QCOMPARE(image.pixel(QPoint(1, 0)), qRgb(127, 127, 127));
        QCOMPARE(image.pixel(QPoint(2, 0)), qRgb(255, 255, 255));
        delete filter;
    }
    else
        QFAIL("The filterGenerator is null!");
    delete newFilterGenerator;
}

/*!
  Test the 0.5% cutoff.
*/

void ut_autolevels::testCutoff()
{
    // Auto levels is actually asymmetrical, so the conditions here
    // are a bit strange.

    QImage image = QImage(QSize(400, 1), QImage::Format_RGB32);

    image.fill(qRgb(128, 128, 128));

    // This should be cut off
    image.setPixel(QPoint(0, 0), qRgb(128, 0, 128));

    // These should not be cut off
    image.setPixel(QPoint(397, 0), qRgb(128, 128, 255));
    image.setPixel(QPoint(398, 0), qRgb(128, 128, 255));
    image.setPixel(QPoint(399, 0), qRgb(128, 128, 255));

    QuillImageFilter *newFilterGenerator =
            QuillImageFilterFactory::createImageFilter("org.maemo.auto.levels");
    QVERIFY(newFilterGenerator);

    QuillImageFilterGenerator *filterGenerator =
            dynamic_cast<QuillImageFilterGenerator*>(newFilterGenerator);
    if(filterGenerator){
        filterGenerator->apply(image);

        QuillImageFilter *filter = filterGenerator->resultingFilter();
        QCOMPARE(filter->name(), QString("com.meego.levels"));

        // Red: no stretch
        QCOMPARE(filter->option(QuillImageFilter::Red).toInt(), 128);
        QCOMPARE(filter->option(QuillImageFilter::RedMax).toInt(), 128);
        // Green: no stretch
        QCOMPARE(filter->option(QuillImageFilter::Green).toInt(), 128);
        QCOMPARE(filter->option(QuillImageFilter::GreenMax).toInt(), 128);
        // Blue: 128 to 255
        QCOMPARE(filter->option(QuillImageFilter::Blue).toInt(), 128);
        QCOMPARE(filter->option(QuillImageFilter::BlueMax).toInt(), 255);
        delete filter;
    }
    else
        QFAIL("The filterGenerator is null!");
    delete newFilterGenerator;
}

/*
  Test preservation of original levels if no stretch is possible.
 */

void ut_autolevels::testNoStretch()
{
    QImage image = QImage(QSize(1, 1), QImage::Format_RGB32);

    image.setPixel(QPoint(0, 0), qRgb(123, 45, 67));

    QuillImageFilter *newFilterGenerator =
            QuillImageFilterFactory::createImageFilter("org.maemo.auto.levels");

    QuillImageFilterGenerator *filterGenerator =
            dynamic_cast<QuillImageFilterGenerator*>(newFilterGenerator);
    if(filterGenerator){
        filterGenerator->apply(image);
        QuillImageFilter *filter = filterGenerator->resultingFilter();
        QImage resultImage = filter->apply(image);
        QCOMPARE(resultImage.pixel(0, 0), qRgb(123, 45, 67));
        delete filter;
    }
    else
        QFAIL("The filterGenerator is null!");
    delete newFilterGenerator;
}


void ut_autolevels::testIsUsedOnPreview()
{
    QuillImageFilter *filter =
            QuillImageFilterFactory::createImageFilter("org.maemo.auto.levels");

    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.auto.levels"));

    QuillImageFilterGenerator *generator =
        dynamic_cast<QuillImageFilterGenerator*>(filter);

    QVERIFY(generator);
    QVERIFY(generator->isUsedOnPreview());

    //delete generator;
    delete filter;
}

void ut_autolevels::testPreserveAlphaChannel()
{
    QuillImage targetImage;
    QuillImage sourceImage(Unittests::generatePaletteImageWithAlpha(8, 8));

    QuillImageFilter *newFilterGenerator =
	    QuillImageFilterFactory::createImageFilter("com.meego.auto.levels");
    QVERIFY(newFilterGenerator);
    QCOMPARE(newFilterGenerator->name(), QString("com.meego.auto.levels"));

    QuillImageFilterGenerator *filterGenerator =
	    dynamic_cast<QuillImageFilterGenerator*>(newFilterGenerator);
    QVERIFY(filterGenerator);

    filterGenerator->apply(sourceImage);

    QuillImageFilter *filter = filterGenerator->resultingFilter();
    QCOMPARE(filter->name(), QString("com.meego.levels"));

    targetImage = filter->apply(sourceImage);

    QRgb *endp_source = (QRgb*)(sourceImage.bits()+sourceImage.numBytes());
    QRgb *p_source = (QRgb*)sourceImage.bits();
    QRgb *p_target = (QRgb*)targetImage.bits();
    for (; p_source < endp_source; p_source++, p_target++)
    {
	QCOMPARE(qAlpha(*p_source), qAlpha(*p_target));
    }

    delete filter;
    delete filterGenerator;
}

int main ( int argc, char *argv[] ){
    QCoreApplication app( argc, argv );
    ut_autolevels test;
    return QTest::qExec( &test, argc, argv );

}
