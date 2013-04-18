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
#include "ut_filterframework.h"

ut_filterframework::ut_filterframework()
{
}

void ut_filterframework::initTestCase()
{
}

void ut_filterframework::cleanupTestCase()
{
}

// Test creation of image filter

void ut_filterframework::testCreation()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.composite.brightness.contrast");
    
    QVERIFY(filter);   
    QCOMPARE(filter->role(), QuillImageFilter::Role_Undefined);
    QCOMPARE(filter->name(), QString("com.meego.composite.brightness.contrast"));

    delete filter;
}

// Test application of image filter

void ut_filterframework::testApply()
{
    QuillImage image(Unittests::generatePaletteImage());
    image.setFullImageSize(QSize(64, 64));
    QCOMPARE(image.fullImageSize(), QSize(64, 64));
    image.setArea(QRect(16, 4, 8, 2));
    QCOMPARE(image.area(), QRect(16, 4, 8, 2));
    
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.composite.brightness.contrast");

    QVERIFY(filter);
    QCOMPARE(filter->role(), QuillImageFilter::Role_Undefined);
    QCOMPARE(filter->name(), QString("com.meego.composite.brightness.contrast"));

    filter->setOption(QuillImageFilter::Brightness, QVariant(20));
    QCOMPARE(filter->option(QuillImageFilter::Brightness), QVariant(20));

    QuillImage imageAfter = filter->apply(image);
    QCOMPARE(imageAfter.fullImageSize(), QSize(64, 64));
    QCOMPARE(imageAfter.area(), QRect(16, 4, 8, 2));

    // Equality has not been reimplemented - is this a bug?
    QCOMPARE(imageAfter, filter->apply(image));

    delete filter;
}

int main ( int argc, char *argv[] ){
    QCoreApplication app( argc, argv );
    ut_filterframework test;
    return QTest::qExec( &test, argc, argv );

}
