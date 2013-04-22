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
#include <QuillImage>
#include <QuillImageFilter>

#include "unittests.h"
#include "ut_quillimage.h"

ut_quillimage::ut_quillimage()
{
}

void ut_quillimage::initTestCase()
{
}

void ut_quillimage::cleanupTestCase()
{
}

// Test

void ut_quillimage::testBasicConstructor()
{
    QuillImage quillImage;

    QCOMPARE(quillImage.fullImageSize(), QSize());
    QCOMPARE(quillImage.area(), QRect());
    QCOMPARE(quillImage.z(), 0);
    QCOMPARE((QImage)quillImage, QImage());
}

void ut_quillimage::testImageConstructors()
{
    QImage image = Unittests::generatePaletteImage();

    QCOMPARE(image.size(), QSize(8, 2));

    QuillImage quillImage(image);

    QCOMPARE(quillImage.fullImageSize(), QSize(8, 2));
    QCOMPARE(quillImage.area(), QRect(QPoint(0, 0), QSize(8, 2)));
    QCOMPARE(quillImage.z(), 0);
    QVERIFY(!quillImage.isFragment());

    QCOMPARE((QImage)quillImage, image);

    
    QuillImage copyQuillImage(quillImage);

    QCOMPARE(copyQuillImage.fullImageSize(), QSize(8, 2));
    QCOMPARE(copyQuillImage.area(), QRect(QPoint(0, 0), QSize(8, 2)));
    QCOMPARE(copyQuillImage.z(), 0);
    QVERIFY(!copyQuillImage.isFragment());
    
    QCOMPARE((QImage)copyQuillImage, image);


    QImage smallImage(QSize(2, 2), QImage::Format_RGB32);
    smallImage.fill(Qt::white);

    QuillImage replaceQuillImage(quillImage, smallImage);

    QCOMPARE(replaceQuillImage.fullImageSize(), QSize(8, 2));
    QCOMPARE(replaceQuillImage.area(), QRect(QPoint(0, 0), QSize(8, 2)));
    QCOMPARE(replaceQuillImage.z(), 0);
    QVERIFY(!replaceQuillImage.isFragment());
    
    QCOMPARE((QImage)replaceQuillImage, smallImage);
}

void ut_quillimage::testFragment()
{
    QImage image(QSize(2, 2), QImage::Format_RGB32);
    image.fill(Qt::white);

    QuillImage quillImage(image);

    QVERIFY(!quillImage.isFragment());

    quillImage.setArea(QRect(0, 0, 1, 1));

    QVERIFY(quillImage.isFragment());
    QCOMPARE(quillImage.area(), QRect(0, 0, 1, 1));

    quillImage.setFullImageSize(QSize(4, 4));

    QVERIFY(quillImage.isFragment());
    QCOMPARE(quillImage.fullImageSize(), QSize(4, 4));

    quillImage.setArea(QRect(0, 0, 4, 4));

    QVERIFY(!quillImage.isFragment());
    QCOMPARE(quillImage.area(), QRect(0, 0, 4, 4));
}

void ut_quillimage::testZ()
{
    QImage image(QSize(2, 2), QImage::Format_RGB32);
    image.fill(Qt::white);

    QuillImage quillImage(image);
    QCOMPARE(quillImage.z(), 0);

    quillImage.setZ(1);
    QCOMPARE(quillImage.z(), 1);
}

void ut_quillimage::testAssignment()
{
    QImage image(QSize(2, 2), QImage::Format_RGB32);
    image.fill(Qt::white);

    QuillImage quillImage2;
    
    {
        QuillImage quillImage(image);
        QCOMPARE(quillImage.fullImageSize(), QSize(2, 2));
        QCOMPARE(quillImage.area(), QRect(0, 0, 2, 2));
        
        quillImage2 = quillImage;
    }

    QCOMPARE(quillImage2.fullImageSize(), QSize(2, 2));
    QCOMPARE(quillImage2.area(), QRect(0, 0, 2, 2));
}

int main ( int argc, char *argv[] ){
    TestApplication app( argc, argv );
    ut_quillimage test;
    return QTest::qExec( &test, argc, argv );

}
