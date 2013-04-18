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
#include "ut_autocontrast.h"

const QLatin1String ut_autocontrast::filterName("com.meego.auto.contrast");
const QLatin1String ut_autocontrast::resultName("com.meego.composite.brightness.contrast");

ut_autocontrast::ut_autocontrast()
{
}

void ut_autocontrast::initTestCase()
{
}

void ut_autocontrast::cleanupTestCase()
{
}

/*!
  Basic auto contrast. Test with three pixels, different shades of grey.
*/

void ut_autocontrast::testGenerator()
{
    QImage image = QImage(QSize(3, 1), QImage::Format_RGB32);

    image.setPixel(QPoint(0, 0), qRgb(64, 64, 64));
    image.setPixel(QPoint(1, 0), qRgb(96, 96, 96));
    image.setPixel(QPoint(2, 0), qRgb(128, 128, 128));

    QuillImageFilter *newFilterGenerator =
        QuillImageFilterFactory::createImageFilter(filterName);
    QVERIFY(newFilterGenerator);
    QCOMPARE(newFilterGenerator->name(), filterName);

    QuillImageFilterGenerator *filterGenerator =
        dynamic_cast<QuillImageFilterGenerator*>(newFilterGenerator);
    QVERIFY(filterGenerator);

    if(filterGenerator){
        filterGenerator->apply(image);

        QuillImageFilter *filter = filterGenerator->resultingFilter();
        QCOMPARE(filter->name(), resultName);

        // Center: 96 to 127.5
        Unittests::compareReal(filter->option(QuillImageFilter::Brightness).toDouble(),
                               31.5/2.55);
        // Contrast component: scale 64 to 255
        Unittests::compareReal(filter->option(QuillImageFilter::Contrast).toDouble(),
                               (255.0-64.0)/64.0*100.0);
        image = filter->apply(image);
        // See that the contrast has really been stretched throughout.
        QCOMPARE(image.pixel(QPoint(0, 0)), qRgb(0, 0, 0));
        QCOMPARE(image.pixel(QPoint(1, 0)), qRgb(128, 128, 128));
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

void ut_autocontrast::testCutoff()
{
    QImage image = QImage(QSize(200, 1), QImage::Format_RGB32);

    image.fill(qRgb(128, 128, 128));

    // This should be cut off
    image.setPixel(QPoint(0, 0), qRgb(0, 0, 0));

    // These should not be cut off
    image.setPixel(QPoint(198, 0), qRgb(255, 255, 255));
    image.setPixel(QPoint(199, 0), qRgb(255, 255, 255));

    QuillImageFilter *newFilterGenerator =
        QuillImageFilterFactory::createImageFilter(filterName);
    QVERIFY(newFilterGenerator);

    QuillImageFilterGenerator *filterGenerator =
        dynamic_cast<QuillImageFilterGenerator*>(newFilterGenerator);
    if(filterGenerator){
        filterGenerator->apply(image);

        QuillImageFilter *filter = filterGenerator->resultingFilter();
        QCOMPARE(filter->name(), resultName);

        // Center: 191.5 to 127.5
        Unittests::compareReal(filter->option(QuillImageFilter::Brightness).toDouble(),
                               -64/2.55);
        // Contrast component: scale 127 to 255
        Unittests::compareReal(filter->option(QuillImageFilter::Contrast).toDouble(),
                               (255.0-127.0)/127.0*100.0);
        delete filter;
    }
    else
        QFAIL("The filterGenerator is null!");
    delete newFilterGenerator;
}

int main ( int argc, char *argv[] ){
    QCoreApplication app( argc, argv );
    ut_autocontrast test;
    return QTest::qExec( &test, argc, argv );

}
