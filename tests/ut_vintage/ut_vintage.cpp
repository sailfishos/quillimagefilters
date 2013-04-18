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
#include "ut_vintage.h"

ut_vintage::ut_vintage()
{
}

void ut_vintage::initTestCase()
{
    refImage = Unittests::generatePaletteImage();
}

void ut_vintage::cleanupTestCase()
{
}

void ut_vintage::testGreyScale()
{
    QuillImageFilter filter("com.meego.greyscale");
    QVERIFY(filter.isValid());
    QCOMPARE(filter.name(), QString("com.meego.greyscale"));

    QImage result = filter.apply(refImage);
    QCOMPARE(result.size(), refImage.size());
}

void ut_vintage::testSepia()
{
    QuillImageFilter filter("com.meego.sepia");
    QVERIFY(filter.isValid());
    QCOMPARE(filter.name(), QString("com.meego.sepia"));

    QImage result = filter.apply(refImage);
    QCOMPARE(result.size(), refImage.size());
}

void ut_vintage::testOldColorPhoto()
{
    QuillImageFilter filter("com.meego.old-color-photo");
    QVERIFY(filter.isValid());
    QCOMPARE(filter.name(), QString("com.meego.old-color-photo"));

    QImage result = filter.apply(refImage);
    QCOMPARE(result.size(), refImage.size());
}

void ut_vintage::testCrossProcessing()
{
    QuillImageFilter filter("com.meego.cross-processing");
    QVERIFY(filter.isValid());
    QCOMPARE(filter.name(), QString("com.meego.cross-processing"));

    QImage result = filter.apply(refImage);
    QCOMPARE(result.size(), refImage.size());
}

int main ( int argc, char *argv[] ){
    QCoreApplication app( argc, argv );
    ut_vintage test;
    return QTest::qExec( &test, argc, argv );

}
