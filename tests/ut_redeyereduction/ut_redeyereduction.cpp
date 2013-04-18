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
#include "quillimagefiltergeneratorimplementation.h"
#include "unittests.h"
#include "ut_redeyereduction.h"

ut_redeyereduction::ut_redeyereduction()
{
}

void ut_redeyereduction::initTestCase()
{
}

void ut_redeyereduction::cleanupTestCase()
{
}

// Test generator properties

void ut_redeyereduction::testRedEyeForCircularArea()
{
    QuillImageFilter *filter =
            QuillImageFilterFactory::createImageFilter("org.maemo.red-eye-reduction");

    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.red-eye-reduction"));

    QPoint center(100, 100);
    QVERIFY(filter->supportsOption(QuillImageFilter::Center));
    QVERIFY(filter->setOption(QuillImageFilter::Center, QVariant(center)));

    QVariantList list0 = filter->option(QuillImageFilter::SelectionArea).value<QVariantList>();
    QVERIFY(list0.count() == 0); // No radius set yet

    for (int r = 1; r < 101; r+=20)
        {
            QVERIFY(filter->supportsOption(QuillImageFilter::Radius));
            QVERIFY(filter->setOption(QuillImageFilter::Radius, QVariant(r)));

            bool bOK = true;
            QCOMPARE(filter->option(QuillImageFilter::Radius).toList().first().toInt(&bOK), r);
            QCOMPARE(filter->option(QuillImageFilter::Center).toList().first().toPoint(), center);

            QVERIFY(bOK);

            QVERIFY(filter->supportsOption(QuillImageFilter::SelectionArea));

            QPolygon polygon = filter->option(QuillImageFilter::SelectionArea).value<QPolygon>();

            float pi = 3.14159265;
            float margin = 1;
            for (int phi = 0; phi < 360; phi += 10)
                {
                    float x = r * cos( 2.0 * pi * phi / 360.0);
                    if (fabs(x) > margin)
                        x = (x>0 ? 1 : -1) * floor(fabs(x) - margin);
                    else
                        x = 0;
                    float y = r * sin( 2.0 * pi * phi / 360.0);
                    if (fabs(y) > margin)
                        y = (y>0 ? 1 : -1) * floor(fabs(y) - margin);
                    else
                        y = 0;
                    QPoint pt(x, y);
                    pt += center;
                    QVERIFY(polygon.containsPoint(pt, Qt::WindingFill));
                }

            for (int phi = 0; phi < 360; phi += 10)
                {
                    float x = r * cos( 2.0 * pi * phi / 360.0);
                    x = (x>0 ? 1 : -1) * ceil(fabs(x) + margin);
                    float y = r * sin( 2.0 * pi * phi / 360.0);
                    y = (y>0 ? 1 : -1) * ceil(fabs(y) + margin);
                    QPoint pt(x, y);
                    pt += center;
                    QVERIFY(polygon.containsPoint(pt, Qt::WindingFill) == false);
                }
        }
    delete filter;
}


void ut_redeyereduction::testRedEyeForMultipleCircularAreas()
{
    QuillImageFilter *filter =
            QuillImageFilterFactory::createImageFilter("org.maemo.red-eye-reduction");

    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.red-eye-reduction"));

    QVariantList centerPoints;
    for (int i = 100; i < 110; i++)
        centerPoints << QPoint(i, i);

    QVERIFY(filter->supportsOption(QuillImageFilter::Center));
    QVERIFY(filter->setOption(QuillImageFilter::Center, QVariant(centerPoints)));

    QVariantList list0 = filter->option(QuillImageFilter::SelectionArea).value<QVariantList>();
    QVERIFY(list0.count() == 0); // No radius set yet

    QVERIFY(filter->supportsOption(QuillImageFilter::Radius));
    QVariantList radii;
    for (int r = 1; r < 101; r++)
        radii << QVariant(r);
    QVERIFY(filter->setOption(QuillImageFilter::Radius, QVariant(radii)));

    QVariantList outRadii = filter->option(QuillImageFilter::Radius).value<QVariantList>();
    for (int i = 0; i < radii.count(); i++) {
            bool bOK = true;
            QCOMPARE(outRadii[i].toInt(), radii[i].toInt(&bOK));
            QVERIFY(bOK);
        }

    QVariantList outCenter = filter->option(QuillImageFilter::Center).value<QVariantList>();
    for (int i = 0; i < centerPoints.count(); i++) {
            QCOMPARE(outCenter[i].toPoint(), centerPoints[i].toPoint());
        }

    QVERIFY(filter->supportsOption(QuillImageFilter::SelectionArea));
    QVariantList polygonList = filter->option(QuillImageFilter::SelectionArea).value<QVariantList>();

    QCOMPARE( polygonList.count(), ( radii.count() < centerPoints.count() ? radii.count() : centerPoints.count() ) );

    for (int i = 0; i < polygonList.count(); i++)
        {
            QPolygon polygon = polygonList[i].value<QPolygon>();

            float pi = 3.14159265;
            float margin = 1;
            for (int phi = 0; phi < 360; phi += 10)
                {
                    float x = radii[i].toInt() * cos( 2.0 * pi * phi / 360.0);
                    if (fabs(x) > margin)
                        x = (x>0 ? 1 : -1) * floor(fabs(x) - margin);
                    else
                        x = 0;
                    float y = radii[i].toInt() * sin( 2.0 * pi * phi / 360.0);
                    if (fabs(y) > margin)
                        y = (y>0 ? 1 : -1) * floor(fabs(y) - margin);
                    else
                        y = 0;
                    QPoint pt(x, y);
                    pt += centerPoints[i].toPoint();
                    QVERIFY(polygon.containsPoint(pt, Qt::WindingFill));
                }

            for (int phi = 0; phi < 360; phi += 10)
                {
                    float x = radii[i].toInt() * cos( 2.0 * pi * phi / 360.0);
                    x = (x>0 ? 1 : -1) * ceil(fabs(x) + margin);
                    float y = radii[i].toInt() * sin( 2.0 * pi * phi / 360.0);
                    y = (y>0 ? 1 : -1) * ceil(fabs(y) + margin);
                    QPoint pt(x, y);
                    pt += centerPoints[i].toPoint();
                    QVERIFY(polygon.containsPoint(pt, Qt::WindingFill) == false);
                }
        }
    delete filter;
}

void ut_redeyereduction::testReductTwoAreas()
{
    QuillImageFilter *filter =
            QuillImageFilterFactory::createImageFilter("org.maemo.red-eye-reduction");

    QVERIFY(filter);
    QCOMPARE(filter->name(), QString("com.meego.red-eye-reduction"));

    QVariantList centerPoints;
    for (int i = 100; i <= 160; i+=60)
        centerPoints << QPoint(i, i);
    QVERIFY(filter->setOption(QuillImageFilter::Center, QVariant(centerPoints)));

    QVariantList radii;
    for (int r = 10; r <= 40; r+=30)
        radii << QVariant(r);
    QVERIFY(filter->setOption(QuillImageFilter::Radius, QVariant(radii)));

    QImage image("/usr/share/quillimagefilter-tests/images/redeye.jpg");
    QVERIFY(!image.isNull());

    QImage image1 = filter->apply(image);
    bool bIsReduced = false;
    for(int h=95; h<105; h++) {
            for(int v =95; v<105; v++) {
                    QRgb color = image.pixel(QPoint(h, v));
                    int limit = (qGreen(color) + qBlue(color)) * 2 / 3;
                    if (qRed(color) > limit) {
                            QVERIFY(qRed(image.pixel(QPoint(h,v))) > (qRed(image1.pixel(QPoint(h,v)))));
                            bIsReduced = true;
                        }
                }
        }
    QVERIFY(bIsReduced);

    bIsReduced = false;
    for(int h=145; h<175; h++) {
            for(int v =145; v<175; v++) {
                    QRgb color = image.pixel(QPoint(h, v));
                    int limit = (qGreen(color) + qBlue(color)) * 2 / 3;
                    if (qRed(color) > limit) {
                            QVERIFY(qRed(image.pixel(QPoint(h,v))) > (qRed(image1.pixel(QPoint(h,v)))));
                            bIsReduced = true;
                        }
                }
        }
    QVERIFY(bIsReduced);

    delete filter;
}



int main ( int argc, char *argv[] ){
    QCoreApplication app( argc, argv );
    ut_redeyereduction test;
    return QTest::qExec( &test, argc, argv );
}
