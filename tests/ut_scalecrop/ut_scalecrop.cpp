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
#include "ut_scalecrop.h"

ut_scalecrop::ut_scalecrop()
{
}

void ut_scalecrop::initTestCase()
{
}

void ut_scalecrop::cleanupTestCase()
{
}

void ut_scalecrop::init()
{
    filter = QuillImageFilterFactory::createImageFilter(QLatin1String("org.maemo.composite.scale.crop"));
    QVERIFY(filter);
}

void ut_scalecrop::cleanup()
{
    delete filter;
}

void ut_scalecrop::testRole()
{
    QCOMPARE(filter->role(), QuillImageFilter::Role_PreviewScale);
}

void ut_scalecrop::testName()
{
    QCOMPARE(filter->name(), QLatin1String("com.meego.composite.scale.crop"));
}

void ut_scalecrop::testCropRectOption()
{
    QVERIFY(filter->supportsOption(QuillImageFilter::CropRectangle));
    QVERIFY(filter->setOption(QuillImageFilter::CropRectangle,
                              QRect(0, 0, 100, 100)));
    QCOMPARE(filter->option(QuillImageFilter::CropRectangle).toRect(),
             QRect(0, 0, 100, 100));
}

void ut_scalecrop::testSizeAfterOption()
{
    QVERIFY(filter->supportsOption(QuillImageFilter::SizeAfter));
    QVERIFY(filter->setOption(QuillImageFilter::SizeAfter,
                              QSize(100, 100)));
    QCOMPARE(filter->option(QuillImageFilter::SizeAfter).toSize(),
             QSize(100, 100));
}

void ut_scalecrop::testFullImageSize()
{
    QCOMPARE(filter->newFullImageSize(QSize(100, 100)), QSize(100, 100));
}

void ut_scalecrop::testArea()
{
    filter->setOption(QuillImageFilter::CropRectangle,
                      QRect(100, 100, 100, 100));
    QCOMPARE(filter->newArea(QSize(300, 300), QRect(0, 0, 300, 300)),
             QRect(100, 100, 100, 100));
}

void ut_scalecrop::testCropOnly()
{
    filter->setOption(QuillImageFilter::CropRectangle,
                      QRect(2, 0, 2, 2));
    QuillImage image = Unittests::generatePaletteImage();
    QuillImage imageAfter = filter->apply(image);

    QCOMPARE(imageAfter.fullImageSize(), image.size());
    QCOMPARE(imageAfter.area(), QRect(2, 0, 2, 2));
    QVERIFY(Unittests::compareImage(imageAfter, image.copy(2, 0, 2, 2)));
}

void ut_scalecrop::testPreviewCrop()
{
    filter->setOption(QuillImageFilter::CropRectangle,
                      QRect(4, 0, 4, 4));
    QuillImage image = Unittests::generatePaletteImage();
    image.setFullImageSize(QSize(16, 4));
    image.setArea(QRect(0, 0, 16, 4));
    QuillImage imageAfter = filter->apply(image);

    QCOMPARE(imageAfter.fullImageSize(), QSize(16, 4));
    QCOMPARE(imageAfter.area(), QRect(4, 0, 4, 4));
    QVERIFY(Unittests::compareImage(imageAfter, image.copy(2, 0, 2, 2)));
}

void ut_scalecrop::testScaleOnly()
{
    filter->setOption(QuillImageFilter::SizeAfter, QSize(4, 1));
    QuillImage image = Unittests::generatePaletteImage();
    QuillImage imageAfter = filter->apply(image);

    QCOMPARE(imageAfter.fullImageSize(), image.size());
    QCOMPARE(imageAfter.area(), QRect(0, 0, 8, 2));
    QVERIFY(Unittests::compareImage(imageAfter, image.scaled(QSize(4, 1))));
}

void ut_scalecrop::testPreviewScale()
{
    filter->setOption(QuillImageFilter::SizeAfter, QSize(4, 1));
    QuillImage image = Unittests::generatePaletteImage();
    image.setFullImageSize(QSize(16, 4));
    image.setArea(QRect(0, 0, 16, 4));
    QuillImage imageAfter = filter->apply(image);

    QCOMPARE(imageAfter.fullImageSize(), QSize(16, 4));
    QCOMPARE(imageAfter.area(), QRect(0, 0, 16, 4));
    QVERIFY(Unittests::compareImage(imageAfter, image.scaled(QSize(4, 1))));
}

void ut_scalecrop::testScaledCrop()
{
    filter->setOption(QuillImageFilter::SizeAfter, QSize(4, 1));
    filter->setOption(QuillImageFilter::CropRectangle, QRect(2, 0, 2, 2));
    QuillImage image = Unittests::generatePaletteImage();
    QuillImage imageAfter = filter->apply(image);

    QCOMPARE(imageAfter.fullImageSize(), image.size());
    QCOMPARE(imageAfter.area(), QRect(2, 0, 2, 2));
    QVERIFY(Unittests::compareImage(imageAfter,
                                    image.copy(QRect(2, 0, 2, 2)).scaled(QSize(4, 1))));
}

void ut_scalecrop::testPreviewScaledCrop()
{
    filter->setOption(QuillImageFilter::SizeAfter, QSize(4, 1));
    filter->setOption(QuillImageFilter::CropRectangle, QRect(4, 0, 4, 4));
    QuillImage image = Unittests::generatePaletteImage();
    image.setFullImageSize(QSize(16, 4));
    image.setArea(QRect(0, 0, 16, 4));
    QuillImage imageAfter = filter->apply(image);

    QCOMPARE(imageAfter.fullImageSize(), QSize(16, 4));
    QCOMPARE(imageAfter.area(), QRect(4, 0, 4, 4));
    QVERIFY(Unittests::compareImage(imageAfter,
                                    image.copy(QRect(2, 0, 2, 2)).scaled(QSize(4, 1))));
}

int main ( int argc, char *argv[] ){
    TestApplication app( argc, argv );
    ut_scalecrop test;
    return QTest::qExec( &test, argc, argv );

}
