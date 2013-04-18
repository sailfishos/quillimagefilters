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
#include <QByteArray>
#include <QuillImageFilter>
#include <QuillImageFilterFactory>

#include "ut_serialsaver.h"
#include "serialsaver.h"
#include "unittests.h"
#include "quillimage.h"

ut_serialsaver::ut_serialsaver()
{
}

void ut_serialsaver::initTestCase()
{
}

void ut_serialsaver::cleanupTestCase()
{
}

// Test standard Qt image saving

void ut_serialsaver::testSerialSaver()
{
    QTemporaryFile testFile;
    testFile.open();
    QByteArray metadata = QByteArray();
    QuillImage image =
        Unittests::generatePaletteImage().transformed(QTransform().rotate(90)).
        scaled(QSize(16, 64));

    SerialSaver serialSaver(testFile.fileName(), image.size(),metadata);

    QVERIFY(serialSaver.process(image.copy(0, 0, 16, 16)));
    QVERIFY(serialSaver.process(image.copy(0, 16, 16, 16)));
    QVERIFY(serialSaver.process(image.copy(0, 32, 16, 16)));
    QVERIFY(serialSaver.process(image.copy(0, 48, 16, 16)));

    QImage resultImage(testFile.fileName());

    Unittests::compareImage(resultImage, image);
}

void ut_serialsaver::testSerialSaverFailure()
{
    QByteArray metadata = QByteArray();
    // Use serial saver with an invalid file name.
    // Without proper error handling, this will force libjpeg to exit the
    // application.
    SerialSaver saver("", QSize(128, 128),metadata);
    QVERIFY(!saver.process(QImage()));
}

int main ( int argc, char *argv[] ){
    QCoreApplication app( argc, argv );
    ut_serialsaver test;
    return QTest::qExec( &test, argc, argv );

}
