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

#include <QuillImageFilter>
#include <QuillImageFilterFactory>
#include "unittests.h"
#include "ut_lanczos.h"

ut_lanczos::ut_lanczos()
{
}

void ut_lanczos::init()
{
    filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.scale.lanczos");
    QVERIFY(filter);
}

void ut_lanczos::cleanup()
{
    delete filter;
}

void ut_lanczos::initTestCase()
{
}

void ut_lanczos::cleanupTestCase()
{
}

void ut_lanczos::testFilterName()
{
    QCOMPARE(filter->name(), QString("com.meego.scale.lanczos"));
}

void ut_lanczos::testRadiusOption()
{
    QVERIFY(filter->supportsOption(QuillImageFilter::Radius));
    QCOMPARE(filter->option(QuillImageFilter::Radius).toInt(), 3);

    QVERIFY(filter->setOption(QuillImageFilter::Radius, QVariant(2)));
    QCOMPARE(filter->option(QuillImageFilter::Radius).toInt(), 2);
}

void ut_lanczos::testTargetSizeOption()
{
    QVERIFY(filter->supportsOption(QuillImageFilter::SizeAfter));
    QVERIFY(!filter->option(QuillImageFilter::SizeAfter).toSize().isValid());

    QVERIFY(filter->setOption(QuillImageFilter::SizeAfter, QSize(16, 16)));
    QCOMPARE(filter->option(QuillImageFilter::SizeAfter).toSize(), QSize(16, 16));
}

void ut_lanczos::testApply()
{
    QVERIFY(filter->setOption(QuillImageFilter::SizeAfter,
                              QVariant(QSize(16, 16))));

    QuillImage image = Unittests::generatePaletteImage();
    QCOMPARE(image.size(), QSize(8, 2));

    QuillImage filteredImage = filter->apply(image);

    // Only verify size, for now

    QCOMPARE(filteredImage.size(), QSize(16, 16));
}

void ut_lanczos::testInvalidRadius()
{
    QVERIFY(!filter->setOption(QuillImageFilter::Radius, QVariant(4)));
    QCOMPARE(filter->option(QuillImageFilter::Radius).toInt(), 3);
}

void ut_lanczos::testInvalidTargetSize()
{
    QuillImage image = Unittests::generatePaletteImage();
    QuillImage filteredImage = filter->apply(image);

    QCOMPARE(image, filteredImage);
}

void ut_lanczos::testInvalidImage()
{
    QuillImage image;
    QVERIFY(filter->setOption(QuillImageFilter::SizeAfter, QSize(16, 16)));
    QuillImage filteredImage = filter->apply(image);

    QCOMPARE(image, filteredImage);
}

int main ( int argc, char *argv[] ){
    TestApplication app( argc, argv );
    ut_lanczos test;
    return QTest::qExec( &test, argc, argv );

}
