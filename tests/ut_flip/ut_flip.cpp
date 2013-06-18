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
#include "ut_flip.h"

ut_flip::ut_flip()
{
}

void ut_flip::initTestCase()
{
}

void ut_flip::cleanupTestCase()
{
}

void ut_flip::testFlip()
{
    QuillImageFilter *filter = new QuillImageFilter("org.maemo.flip");

    QVERIFY(filter->isValid());

    QCOMPARE(filter->name(), QString("com.meego.flip"));

    QVERIFY(filter->supportsOption(QuillImageFilter::Horizontal));
    QVERIFY(filter->setOption(QuillImageFilter::Horizontal, QVariant(true)));

    QCOMPARE(filter->option(QuillImageFilter::Horizontal).toBool(), true);

    QImage image = Unittests::generatePaletteImage();
    QCOMPARE(image.size(), QSize(8, 2));

    QImage filteredImage = filter->apply(image);

    QCOMPARE(filteredImage.size(), QSize(8, 2));

    // Verify against Qt's own flip

    QCOMPARE(filteredImage,
             image.transformed(QTransform().scale(-1, 1)));

    QVERIFY(filter->setOption(QuillImageFilter::Horizontal, QVariant(false)));

    filteredImage = filter->apply(image);

    QCOMPARE(filteredImage.size(), QSize(8, 2));

    // Verify against Qt's own flip

    QCOMPARE(filteredImage,
             image.transformed(QTransform().scale(1, -1)));

    delete filter;
}

// Flip + flip + rotate 180 should result in original.
void ut_flip::testFlipRotate()
{
    QuillImageFilter *flip = new QuillImageFilter("org.maemo.flip");

    QuillImageFilter *flip2 = new QuillImageFilter("org.maemo.flip");

    flip2->setOption(QuillImageFilter::Horizontal, QVariant(true));

    QuillImageFilter *rotate = new QuillImageFilter("org.maemo.rotate");

    rotate->setOption(QuillImageFilter::Angle, QVariant(180));

    QuillImage image = Unittests::generatePaletteImage();
    QCOMPARE(image, rotate->apply(flip2->apply(flip->apply(image))));

    delete flip;
    delete flip2;
    delete rotate;
}

void ut_flip::testQuillImage()
{
    QuillImage image(Unittests::generatePaletteImage());
    image.setFullImageSize(QSize(16, 4));
    image.setArea(QRect(0, 0, 8, 2));

    QuillImageFilter *filter = new QuillImageFilter("org.maemo.flip");
    QVERIFY(filter->isValid());

    // Flip vertical
    QuillImage image2 = filter->apply(image);

    QCOMPARE((QImage)image2, image.mirrored(false, true));
    QCOMPARE(image2.fullImageSize(), QSize(16, 4));
    QCOMPARE(image2.area(), QRect(0, 2, 8,2));

    // Flip horizontal
    filter->setOption(QuillImageFilter::Horizontal, true);
    QuillImage image3 = filter->apply(image2);

    QCOMPARE((QImage)image3, image.mirrored(true, true));
    QCOMPARE(image3.fullImageSize(), QSize(16, 4));
    QCOMPARE(image3.area(), QRect(8, 2, 8, 2));

    delete filter;
}


int main ( int argc, char *argv[] ){
    TestApplication app( argc, argv );
    ut_flip test;
    return QTest::qExec( &test, argc, argv );

}
