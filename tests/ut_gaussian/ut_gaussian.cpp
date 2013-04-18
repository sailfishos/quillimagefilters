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
#include "ut_gaussian.h"

ut_gaussian::ut_gaussian()
{
}

void ut_gaussian::initTestCase()
{
}

void ut_gaussian::cleanupTestCase()
{
}

void ut_gaussian::testFilter()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.blur.gaussian");

    QVERIFY(filter);

    QCOMPARE(filter->name(), QString("com.meego.blur.gaussian"));

    QVERIFY(filter->supportsOption(QuillImageFilter::Radius));
    QCOMPARE(filter->option(QuillImageFilter::Radius).toDouble(), 0.0);

    QVERIFY(filter->setOption(QuillImageFilter::Radius, QVariant(1.0)));
    QCOMPARE(filter->option(QuillImageFilter::Radius).toDouble(), 1.0);

    delete filter;
}

void ut_gaussian::testApply()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.blur.gaussian");
    QVERIFY(filter->setOption(QuillImageFilter::Radius, QVariant(1.0)));

    QuillImage image = Unittests::generatePaletteImage();
    QCOMPARE(image.size(), QSize(8, 2));

    QImage filteredImage = filter->apply(image);

    // Only verify size, for now

    QCOMPARE(filteredImage.size(), QSize(8, 2));

    delete filter;
}

void ut_gaussian::testInvalidRadius()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.blur.gaussian");
    QVERIFY(filter->setOption(QuillImageFilter::Radius, QVariant(0.0)));

    QuillImage image = Unittests::generatePaletteImage();
    QCOMPARE(image.size(), QSize(8, 2));

    QImage filteredImage = filter->apply(image);

    // Only verify size, for now

    QCOMPARE(filteredImage.size(), QSize(8, 2));

    delete filter;
}

void ut_gaussian::testInvalidImage()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.blur.gaussian");
    QVERIFY(filter->setOption(QuillImageFilter::Radius, QVariant(0.0)));

    QuillImage image;

    QImage filteredImage = filter->apply(image);

    QVERIFY(filteredImage.isNull());

    delete filter;
}

int main ( int argc, char *argv[] ){
    QCoreApplication app( argc, argv );
    ut_gaussian test;
    return QTest::qExec( &test, argc, argv );

}
