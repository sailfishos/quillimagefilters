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
#include <QRect>
#include <QuillImageFilter>
#include <QuillImageFilterFactory>
#include <QuillImageFilterGenerator>
#include "unittests.h"
#include "ut_redeyedetection.h"

ut_redeyedetection::ut_redeyedetection()
{
}

void ut_redeyedetection::initTestCase()
{
}

void ut_redeyedetection::cleanupTestCase()
{
}

// Test generator properties

void ut_redeyedetection::testGenerator()
{
    QuillImageFilter *filter =
        QuillImageFilterFactory::createImageFilter("org.maemo.red-eye-detection");

    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.red-eye-detection"));

    QVERIFY(filter->supportsOption(QuillImageFilter::Center));
    QVERIFY(filter->setOption(QuillImageFilter::Center, QVariant(QPoint(108,86))));

    QVERIFY(filter->supportsOption(QuillImageFilter::Radius));
    QVERIFY(filter->setOption(QuillImageFilter::Radius, QVariant(40)));


    bool bOK = true;
    QCOMPARE(filter->option(QuillImageFilter::Radius).toInt(&bOK), 40);
    QCOMPARE(filter->option(QuillImageFilter::Center).toPoint(), QPoint(108,86));

    QVERIFY(bOK);

    QImage image("/usr/share/quillimagefilter-tests/images/redeye.jpg");

    QVERIFY(!image.isNull());

    QuillImageFilterGenerator *filterGenerator =
        dynamic_cast<QuillImageFilterGenerator*>(filter);
    QVERIFY(filterGenerator);

    if(filterGenerator){
        filterGenerator->apply(image);

        QuillImageFilter *filter1 = filterGenerator->resultingFilter();
        QVERIFY(filter1);
        QCOMPARE(filter1->name(), QString("com.meego.red-eye-reduction"));

        QVERIFY(filter1->supportsOption(QuillImageFilter::SelectionArea));
        QRect area = filter1->option(QuillImageFilter::SelectionArea).value<QPolygon>().boundingRect();
        // Target rect is QRect(QPoint(70, 67), QSize(60, 41)). Allow a
        // tolerance of +/2.
        QVERIFY(area.top() > 64);
        QVERIFY(area.top() < 70);
        QVERIFY(area.left() > 67);
        QVERIFY(area.left() < 73);
        QVERIFY(area.bottom() > 104);
        QVERIFY(area.bottom() < 110);
        QVERIFY(area.right() > 126);
        QVERIFY(area.right() < 132);
        QImage image1 = filter1->apply(image);

        bool bIsReduced = false;
        for(int h=115; h<119;h++) {
                for(int v =85; v<87; v++) {
                        QRgb color = image.pixel(QPoint(h, v));
                        int limit = (qGreen(color) + qBlue(color)) * 2 / 3;
                        if (qRed(color) > limit) {
                                QVERIFY(qRed(image.pixel(QPoint(h,v))) > (qRed(image1.pixel(QPoint(h,v)))));
                                bIsReduced = true;
                            }
                    }
            }
        QVERIFY(bIsReduced);

        QCOMPARE(filterGenerator->isUsedOnPreview(), false);
        delete filter1;
    }
    else
        QFAIL("The filterGenerator is null!");
    delete filter;
}

void ut_redeyedetection::testIsUsedOnPreview()
{
    QuillImageFilter *filter =
            QuillImageFilterFactory::createImageFilter("org.maemo.red-eye-detection");

    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.red-eye-detection"));

    QuillImageFilterGenerator *generator =
        dynamic_cast<QuillImageFilterGenerator*>(filter);

    QVERIFY(generator);
    QVERIFY(!generator->isUsedOnPreview());

    //delete generator;
    delete filter;
}

int main ( int argc, char *argv[] ){
    QCoreApplication app( argc, argv );
    ut_redeyedetection test;
    return QTest::qExec( &test, argc, argv );
}
