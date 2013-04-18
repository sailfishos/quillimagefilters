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
#include <QIODevice>
#include <QFile>
#include <QuillImageFilter>
#include <QuillImageFilterFactory>

#include "ut_save.h"
#include "unittests.h"
#include "quillimagefilterimplementation.h"

ut_save::ut_save()
{
}

void ut_save::initTestCase()
{
}

void ut_save::cleanupTestCase()
{
}


void ut_save::testRole()
{
    QuillImageFilter filter("org.maemo.save");
    QCOMPARE(filter.role(), QuillImageFilter::Role_Save);
}

// Test standard Qt image saving

void ut_save::testStandardPNG()
{
    QTemporaryFile testFile;
    testFile.open();
    QString fileName = testFile.fileName();

    QImage image = Unittests::generatePaletteImage();
    image.save(fileName, "png");

    QImage loadedImage(fileName);
    QCOMPARE(loadedImage, Unittests::generatePaletteImage());
}

void ut_save::testQImage()
{
    QTemporaryFile testFile;
    testFile.open();

    QuillImageFilter *filter = new QuillImageFilter("org.maemo.save");
        //QuillImageFilterFactory::createImageFilter("org.maemo.save");
    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.save"));
    QVERIFY(filter->supportsOption(QuillImageFilter::FileName));
    QVERIFY(filter->supportsOption(QuillImageFilter::FileFormat));
    QVERIFY(filter->supportsOption(QuillImageFilter::DisablePreview));

    filter->setOption(QuillImageFilter::FileName,
                      QVariant(testFile.fileName()));
    filter->setOption(QuillImageFilter::FileFormat,
                      QVariant("png"));

    QCOMPARE(filter->option(QuillImageFilter::FileName).toString(),
             testFile.fileName());
    QCOMPARE(filter->option(QuillImageFilter::FileFormat).toString(),
             QString("png"));

    QImage image = Unittests::generatePaletteImage();
    QVERIFY(!filter->apply(image).isNull());

    QImage savedImage(testFile.fileName());
    QVERIFY(Unittests::compareImage(savedImage, image));

    testFile.close();
    delete filter;
}
void ut_save::testIODevice()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.save");
    QVERIFY(filter);
    QFile *iODevice = new QFile("savedImage.png");
    QVERIFY(iODevice);
    QVERIFY(iODevice->open(QIODevice::WriteOnly));
    QCOMPARE(iODevice->isOpen(),true);
    QCOMPARE(iODevice->isReadable(),false);
    QCOMPARE(iODevice->isWritable(),true);
    filter->setIODevice(iODevice);
    QCOMPARE(iODevice, filter->iODevice());
    QImage image = Unittests::generatePaletteImage();
    QuillImage savedImage = filter->apply(image);
    QVERIFY(Unittests::compareImage(savedImage, QuillImage(image)));
    delete iODevice;
    delete filter;
}

void ut_save::testQuillImage()
{
    QTemporaryFile testFile;
    testFile.open();

    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.save");
    QVERIFY(filter);
    QVERIFY(filter->supportsOption(QuillImageFilter::TileCount));

    filter->setOption(QuillImageFilter::FileName,
                      QVariant(testFile.fileName()));
    filter->setOption(QuillImageFilter::FileFormat,
                      QVariant("png"));

    QImage image = Unittests::generatePaletteImage();

    // Setup tiles

    QuillImage image1(image.copy(0, 0, 2, 2));
    image1.setFullImageSize(QSize(8, 2));
    image1.setArea(QRect(0, 0, 2, 2));

    QuillImage image2(image.copy(2, 0, 2, 2));
    image2.setFullImageSize(QSize(8, 2));
    image2.setArea(QRect(2, 0, 2, 2));

    QuillImage image3(image.copy(4, 0, 2, 2));
    image3.setFullImageSize(QSize(8, 2));
    image3.setArea(QRect(4, 0, 2, 2));

    QuillImage image4(image.copy(6, 0, 2, 2));
    image4.setFullImageSize(QSize(8, 2));
    image4.setArea(QRect(6, 0, 2, 2));

    filter->setOption(QuillImageFilter::TileCount, QVariant(4));

    // Require that all save filters return a non-empty result
    // (i.e. they succeed)

    QVERIFY(!filter->apply(image1).isNull());
    QCOMPARE(filter->option(QuillImageFilter::TileCount).toInt(), 3);

    QVERIFY(!filter->apply(image2).isNull());
    QCOMPARE(filter->option(QuillImageFilter::TileCount).toInt(), 2);

    QVERIFY(!filter->apply(image3).isNull());
    QCOMPARE(filter->option(QuillImageFilter::TileCount).toInt(), 1);

    QVERIFY(!filter->apply(image4).isNull());
    QCOMPARE(filter->option(QuillImageFilter::TileCount).toInt(), 0);

    QImage savedImage(testFile.fileName());
    QVERIFY(Unittests::compareImage(savedImage, image));

    //We test IO device here

    QImage image5 = Unittests::generatePaletteImage();

    // Setup tiles

    QuillImage image6(image5.copy(0, 0, 2, 2));
    image6.setFullImageSize(QSize(8, 2));
    image6.setArea(QRect(0, 0, 2, 2));

    QuillImage image7(image5.copy(2, 0, 2, 2));
    image7.setFullImageSize(QSize(8, 2));
    image7.setArea(QRect(2, 0, 2, 2));

    QuillImage image8(image5.copy(4, 0, 2, 2));
    image8.setFullImageSize(QSize(8, 2));
    image8.setArea(QRect(4, 0, 2, 2));

    QuillImage image9(image5.copy(6, 0, 2, 2));
    image9.setFullImageSize(QSize(8, 2));
    image9.setArea(QRect(6, 0, 2, 2));

    filter->setOption(QuillImageFilter::TileCount, QVariant(4));
    QIODevice *iODevice = new QFile(testFile.fileName());
    QVERIFY(iODevice);
    filter->setIODevice(iODevice);
    QCOMPARE(iODevice, filter->iODevice());
    // Require that all save filters return a non-empty result
    // (i.e. they succeed)

    QVERIFY(!filter->apply(image6).isNull());
    QCOMPARE(filter->option(QuillImageFilter::TileCount).toInt(), 3);

    QVERIFY(!filter->apply(image7).isNull());
    QCOMPARE(filter->option(QuillImageFilter::TileCount).toInt(), 2);

    QVERIFY(!filter->apply(image8).isNull());
    QCOMPARE(filter->option(QuillImageFilter::TileCount).toInt(), 1);

    QVERIFY(!filter->apply(image9).isNull());
    QCOMPARE(filter->option(QuillImageFilter::TileCount).toInt(), 0);

    QVERIFY(Unittests::compareImage(savedImage, image5));
    delete iODevice;
    //The IO device test ends
    delete filter;
}

void ut_save::testRoleAndDomainName()
{
    QuillImageFilter *filter =
    QuillImageFilterFactory::createImageFilter(QuillImageFilter::Role_Save);

    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.save"));
    delete filter;
}

void ut_save::testFailedSave()
{
    QTemporaryFile testFile;
    testFile.open();
    testFile.setPermissions(QFile::ReadOwner); // set file to read only

    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter(QuillImageFilter::Role_Save);
    QVERIFY(filter);

    filter->setOption(QuillImageFilter::FileName,
                      QVariant(testFile.fileName()));

    QuillImage image = Unittests::generatePaletteImage();

    // Verify failure
    QVERIFY(filter->apply(image).isNull());

    delete filter;
}

int main ( int argc, char *argv[] ){
    QCoreApplication app( argc, argv );
    ut_save test;
    return QTest::qExec( &test, argc, argv );

}
