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
#include "ut_overlay.h"

ut_overlay::ut_overlay()
{
}

void ut_overlay::initTestCase()
{
}

void ut_overlay::cleanupTestCase()
{
}

void ut_overlay::testRole()
{
    QuillImageFilter filter("org.maemo.overlay");
    QCOMPARE(filter.role(), QuillImageFilter::Role_Overlay);
}

void ut_overlay::testBasics()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.overlay");

    QCOMPARE(filter->name(), QString("com.meego.overlay"));
    QVERIFY(filter->supportsOption(QuillImageFilter::Background));
    QVERIFY(filter->supportsOption(QuillImageFilter::CropRectangle));

    QImage image = Unittests::generatePaletteImage();

    filter->setOption(QuillImageFilter::Background,
                      image);
    filter->setOption(QuillImageFilter::CropRectangle,
                      QRect(0, 0, 8, 2));

    QCOMPARE(filter->option(QuillImageFilter::Background).value<QImage>(),
             image);

    QCOMPARE(filter->option(QuillImageFilter::CropRectangle).toRect(),
             QRect(0, 0, 8, 2));

    delete filter;
}

void ut_overlay::testOverlay()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.overlay");

    QImage image = Unittests::generatePaletteImageWithAlpha(8, 2);

    filter->setOption(QuillImageFilter::Background,
                      image);
    filter->setOption(QuillImageFilter::CropRectangle,
                      QRect(0, 2, 8, 2));

    QuillImage tile = QuillImage(QImage(QSize(2, 2), QImage::Format_ARGB32));
    tile.fill(qRgba(255, 255, 255, 255));
    tile.setArea(QRect(2, 2, 2, 2));

    QuillImage buffer = filter->apply(tile);
    QVERIFY(buffer.pixel(2, 0) !=  image.pixel(2, 0));
    QVERIFY(buffer.pixel(2, 1) !=  image.pixel(2, 1));
    QVERIFY(buffer.pixel(3, 0) !=  image.pixel(3, 0));
    QVERIFY(buffer.pixel(3, 1) !=  image.pixel(3, 1));

    QCOMPARE(buffer.pixel(1, 1), image.pixel(1, 1));
    QCOMPARE(buffer.pixel(4, 0), image.pixel(4, 0));

    delete filter;
}

void ut_overlay::testRoleAndDomainName()
{
    QuillImageFilter *filter =
    QuillImageFilterFactory::createImageFilter( QuillImageFilter::Role_Overlay);
    
    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.overlay"));
    delete filter;
}

int main ( int argc, char *argv[] ){
    QCoreApplication app( argc, argv );
    ut_overlay test;
    return QTest::qExec( &test, argc, argv );
}
