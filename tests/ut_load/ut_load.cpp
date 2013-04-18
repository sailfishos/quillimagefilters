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
#include <QuillMetadata>
#include <QuillImageFilter>
#include <QuillImageFilterFactory>

#include "ut_load.h"
#include "unittests.h"

ut_load::ut_load()
{
}

void ut_load::initTestCase()
{
}

void ut_load::cleanupTestCase()
{
}


void ut_load::testRole()
{
    QuillImageFilter filter("org.maemo.load");
    QCOMPARE(filter.role(), QuillImageFilter::Role_Load);
}

void ut_load::testQImage()
{
    QFile file("/usr/share/quillimagefilter-tests/images/16_color_palette.png");
    QuillImageFilter *filter =
            new QuillImageFilter("com.meego.load");
    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.load"));
    QVERIFY(filter->supportsOption(QuillImageFilter::FileName));
    QVERIFY(filter->supportsOption(QuillImageFilter::FileFormat));
    QVERIFY(filter->supportsOption(QuillImageFilter::DisableUndo));
    QVERIFY(filter->supportsOption(QuillImageFilter::SizeAfter));

    filter->setOption(QuillImageFilter::FileName,
                      QVariant(file.fileName()));
    filter->setOption(QuillImageFilter::MimeType,
                      QVariant("image/png"));

    QCOMPARE(filter->option(QuillImageFilter::FileName).toString(),
             file.fileName());
    QCOMPARE(filter->option(QuillImageFilter::MimeType).toString(),
             QString("image/png"));
    QCOMPARE(filter->option(QuillImageFilter::FileFormat).toString(),
             QString("png"));

    QImage image(file.fileName());
    QImage loadedImage = filter->apply(image);

    QVERIFY(Unittests::compareImage(loadedImage, QuillImage(image)));

    delete filter;
}

void ut_load::testPluginQImage()
{

    QFile file("/usr/share/quillimagefilter-tests/images/16_color_palette.png");
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");
    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.load"));
    QVERIFY(filter->supportsOption(QuillImageFilter::FileName));
    QVERIFY(filter->supportsOption(QuillImageFilter::FileFormat));
    QVERIFY(filter->supportsOption(QuillImageFilter::DisableUndo));
    QVERIFY(filter->supportsOption(QuillImageFilter::SizeAfter));

    filter->setOption(QuillImageFilter::FileName,
                      QVariant(file.fileName()));
    filter->setOption(QuillImageFilter::MimeType,
                      QVariant("image/png"));

    QCOMPARE(filter->option(QuillImageFilter::FileName).toString(),
             file.fileName());
    QCOMPARE(filter->option(QuillImageFilter::MimeType).toString(),
             QString("image/png"));
    QCOMPARE(filter->option(QuillImageFilter::FileFormat).toString(),
             QString("png"));

    QImage image(file.fileName());
    QImage loadedImage = filter->apply(image);

    QVERIFY(Unittests::compareImage(loadedImage, QuillImage(image)));

    delete filter;
}

void ut_load::testIODevice()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");
    QFile *fileIO = new QFile("/usr/share/quillimagefilter-tests/images/16_color_palette.png");
    QVERIFY(fileIO->open(QIODevice::ReadOnly));
    QCOMPARE(fileIO->isOpen(),true);
    QCOMPARE(fileIO->isReadable(),true);
    QVERIFY(fileIO);
    filter->setIODevice(fileIO);
    QCOMPARE(fileIO, filter->iODevice());
    QImage image("/usr/share/quillimagefilter-tests/images/16_color_palette.png");
    QImage loadedImage1 = filter->apply(image);
    QVERIFY(Unittests::compareImage(loadedImage1, image));
    delete fileIO;
    delete filter;
}

void ut_load::testRoleAndDomainName()
{
    QuillImageFilter *filter =
    QuillImageFilterFactory::createImageFilter(QuillImageFilter::Role_Load);

    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.load"));
    delete filter;
}

void ut_load::testFileNotFound()
{
    QFile fooFile("/usr/share/quillimagefilter-tests/images/fooxxx.png");
    QVERIFY(!fooFile.exists());

    QImage fooImage(fooFile.fileName());
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");
    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.load"));
    QVERIFY(filter->supportsOption(QuillImageFilter::FileName));
    QVERIFY(filter->supportsOption(QuillImageFilter::FileFormat));
    QVERIFY(filter->supportsOption(QuillImageFilter::DisableUndo));
    QVERIFY(filter->supportsOption(QuillImageFilter::SizeAfter));

    filter->setOption(QuillImageFilter::FileName,
                      QVariant(fooFile.fileName()));
    filter->setOption(QuillImageFilter::MimeType,
                      QVariant("image/png"));

    filter->apply(fooImage);
    QCOMPARE(filter->error(),QuillImageFilter::FileNotFoundError);
    fooFile.close();
    delete filter;
}

void ut_load::testUnsupportedFormat()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");
    QFile unsupportedFile("/usr/share/quillimagefilter-tests/images/16_color_palette.xcf");
    QImage unsupportedImage(unsupportedFile.fileName());
    filter->setOption(QuillImageFilter::FileName,
                      QVariant( unsupportedFile.fileName()));
    filter->setOption(QuillImageFilter::FileFormat,
                      QVariant("xcf"));
    filter->apply(unsupportedImage);
    QCOMPARE(filter->error(),QuillImageFilter::UnsupportedFormatError);
    unsupportedFile.close();
    delete filter;
}

void ut_load:: testInvalidData()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");
    QFile invalidFile("/usr/share/quillimagefilter-tests/images/16_color_palette.xcf");
    QImage invalidImage(invalidFile.fileName());
    filter->setOption(QuillImageFilter::FileName,
                      QVariant( invalidFile.fileName()));
    filter->setOption(QuillImageFilter::MimeType,
                      QVariant("image/png"));
    filter->apply(invalidImage);
    QCOMPARE(filter->error(),QuillImageFilter::InvalidDataError);
    invalidFile.close();
    delete filter;
}

void ut_load::testFormatDetectionFromSuffix()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");

    QFile file("/usr/share/quillimagefilter-tests/images/16_color_palette.png");
    filter->setOption(QuillImageFilter::FileName, QString(file.fileName()));

    QCOMPARE(filter->option(QuillImageFilter::FileFormat).toString(), QString("png"));
    QCOMPARE(filter->option(QuillImageFilter::MimeType).toString(), QString("image/png"));
    delete filter;
}


void ut_load::testFormatDetectionFromContents()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");

    QTemporaryFile testFile;
    testFile.open();

    Unittests::generatePaletteImage().save(testFile.fileName(), "jpg");

    filter->setOption(QuillImageFilter::FileName, QString(testFile.fileName()));

    QCOMPARE(filter->option(QuillImageFilter::FileFormat).toString(), QString("jpeg"));
    QCOMPARE(filter->option(QuillImageFilter::MimeType).toString(), QString("image/jpeg"));
    delete filter;
}

void ut_load::testLoadError()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");

    QTemporaryFile testFile;
    testFile.open();

    filter->setOption(QuillImageFilter::FileName, testFile.fileName());
    filter->setOption(QuillImageFilter::FileFormat, QString("png"));

    QCOMPARE(filter->option(QuillImageFilter::FileFormat).toString(),
             QString("png"));

    QSize size = filter->newFullImageSize(QSize());
    QVERIFY(!size.isValid());

    QCOMPARE((int)filter->error(), (int)QuillImageFilter::InvalidDataError);

    delete filter;
}

void ut_load::testTransparency()
{
    QTemporaryFile testFile;
    testFile.open();

    QImage image = Unittests::generatePaletteImage();
    image = image.convertToFormat(QImage::Format_ARGB32);
    image.setPixel(QPoint(0, 0), qRgba(255, 0, 0, 0));
    image.setPixel(QPoint(0, 1), qRgba(255, 0, 0, 127));

    image.save(testFile.fileName(), "png");

    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");

    filter->setOption(QuillImageFilter::FileName, testFile.fileName());
    filter->setOption(QuillImageFilter::FileFormat, QString("png"));
    filter->setOption(QuillImageFilter::BackgroundColor, QColor(qRgb(0, 0, 255)));

    QuillImage imageAfter = filter->apply(image);

    QCOMPARE(imageAfter.pixel(QPoint(0, 0)), qRgb(0, 0, 255));
    QCOMPARE(imageAfter.pixel(QPoint(0, 1)), qRgb(127, 0, 128));

    delete filter;
}

void ut_load::testCropping()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");

    QFile file("/usr/share/quillimagefilter-tests/images/16_color_palette.png");
    filter->setOption(QuillImageFilter::FileName, QString(file.fileName()));

    QuillImage target = QuillImage();
    target.setTargetSize(QSize(1, 1));
    target.setFullImageSize(QSize(8, 2));
    target.setArea(QRect(2, 0, 2, 2));

    QuillImage result = filter->apply(target);
    QCOMPARE(result.size(), QSize(1, 1));
    // Expect an average of the given area (png = smooth scaling).
    QCOMPARE(qRed(result.pixel(QPoint(0,0))), 96);
    QCOMPARE(qGreen(result.pixel(QPoint(0,0))), 193);
    QCOMPARE(qBlue(result.pixel(QPoint(0,0))), 0);

    delete filter;
}

void ut_load::testExifRotationSizePeek()
{
    QTemporaryFile testFile;
    testFile.open();

    QImage image("/usr/share/quillimagefilter-tests/images/16_color_palette.png");
    image.save(testFile.fileName(), "jpeg");

    QuillMetadata metadata;
    metadata.setEntry(QuillMetadata::Tag_Orientation, QVariant(6));
    metadata.write(testFile.fileName());

    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");

    filter->setOption(QuillImageFilter::FileName, QString(testFile.fileName()));
    QCOMPARE(filter->newFullImageSize(QSize()), QSize(2, 8));

    delete filter;
}

void ut_load::testExifRotation()
{
    QTemporaryFile testFile;
    testFile.open();

    QImage image("/usr/share/quillimagefilter-tests/images/16_color_palette.png");
    image.save(testFile.fileName(), "jpeg");

    QuillMetadata metadata;
    metadata.setEntry(QuillMetadata::Tag_Orientation, QVariant(6));
    metadata.write(testFile.fileName());

    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");

    filter->setOption(QuillImageFilter::FileName, QString(testFile.fileName()));
    QuillImage resultImage = filter->apply(QuillImage());
    QCOMPARE(resultImage.size(), QSize(2, 8));

    delete filter;
}

void ut_load::testLoadSameFilterTwice()
{
    QTemporaryFile testFile;
    testFile.open();
    QImage greenTestImage("/usr/share/quillimagefilter-tests/images/854x480_blue.png");
    // Verify whether image exists
    QVERIFY(!greenTestImage.isNull());
    greenTestImage.save(testFile.fileName(), "jpg");

    QuillImage target = QuillImage();
    target.setFullImageSize(QSize(854, 480));
    target.setArea(QRect(0, 0, 854, 480));

    QuillImage target2 = QuillImage();
    target2.setFullImageSize(QSize(854, 480));
    target2.setArea(QRect(0, 0, 854, 480));

    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");

    filter->setOption(QuillImageFilter::FileName, testFile.fileName());

    // Passing JPEG through the filter
    QuillImage imageAfter = filter->apply(target);

    // Passing JPEG through the filter
    QuillImage imageAfter2 = filter->apply(target2);

    // Checking the red channel value which should be always bigger than that of greeen channel
    QVERIFY( qBlue(imageAfter.pixel(QPoint(0, 0))) > qGreen(imageAfter.pixel(QPoint(0, 0))) );
    // Checking the red channel value which should be always bigger than that of greeen channel
    QVERIFY( qBlue(imageAfter2.pixel(QPoint(0, 0))) > qGreen(imageAfter2.pixel(QPoint(0, 0))) );
    delete filter;
}

void ut_load::testLoadSetTargetSizeOnly()
{
    QTemporaryFile testFile;
    testFile.open();

    Unittests::generatePaletteImage().save(testFile.fileName(), "jpg");

    QuillImageFilter filter("com.meego.load");

    filter.setOption(QuillImageFilter::FileName,
                     QVariant(testFile.fileName()));
    filter.setOption(QuillImageFilter::MimeType,
                     QVariant("image/jpeg"));

    QuillImage image;
    image.setTargetSize(QSize(4, 1));
    QuillImage loadedImage = filter.apply(image);

    QCOMPARE(loadedImage.size(), QSize(4, 1));
}


int main ( int argc, char *argv[] ){
    QCoreApplication app( argc, argv );
    ut_load test;
    return QTest::qExec( &test, argc, argv );

}
