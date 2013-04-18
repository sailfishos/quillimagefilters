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

#include <QCoreApplication>
#include <QImage>
#include <QTime>
#include <QTemporaryFile>
#include <QPen>
#include <QPolygon>
#include <QFont>
#include <QDebug>
#include <QuillImageFilter>
#include <QuillImageFilterFactory>
#include <QuillImageFilterGenerator>

/* Filters benchmark */

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Brightness +20%

    QuillImageFilter *f1 =
        QuillImageFilterFactory::createImageFilter("org.maemo.composite.brightness.contrast");
    f1->setOption(QuillImageFilter::Brightness, QVariant(20));

    // Contrast +20%

    QuillImageFilter *f2 =
        QuillImageFilterFactory::createImageFilter("org.maemo.composite.brightness.contrast");
    f2->setOption(QuillImageFilter::Contrast, QVariant(20));

    // Levels stretch

    QuillImageFilter *f3 =
        QuillImageFilterFactory::createImageFilter("org.maemo.levels");
    f3->setOption(QuillImageFilter::Red, QVariant(10));
    f3->setOption(QuillImageFilter::RedMax, QVariant(240));
    f3->setOption(QuillImageFilter::Green, QVariant(10));
    f3->setOption(QuillImageFilter::GreenMax, QVariant(240));
    f3->setOption(QuillImageFilter::Blue, QVariant(10));
    f3->setOption(QuillImageFilter::BlueMax, QVariant(240));

    // Color balance
    QuillImageFilter *f4 =
        QuillImageFilterFactory::createImageFilter("org.maemo.color-balance");
    if (f4)
        f4->setOption(QuillImageFilter::Red,QVariant(40));

    // Flip
    QuillImageFilter *f5 =
        QuillImageFilterFactory::createImageFilter("org.maemo.flip");
    f5->setOption(QuillImageFilter::Horizontal, QVariant(true));

    // Rotate 90* clockwise

    QuillImageFilter *f6 =
        QuillImageFilterFactory::createImageFilter("org.maemo.rotate");

    // Crop halving each dimension, center at center of image

    QuillImageFilter *f7 =
        QuillImageFilterFactory::createImageFilter("org.maemo.crop");

    // Full image size is 6000x4500
    f7->setOption(QuillImageFilter::CropRectangle,
                  QVariant(QRect(QPoint(1500, 1125), QSize(3000, 2250))));

    QuillImageFilter *f8 =
        QuillImageFilterFactory::createImageFilter("org.maemo.free-rotate");

    f8->setOption(QuillImageFilter::Angle, QVariant(10));

    // High-quality rescale halving each dimension

    QuillImageFilter *f9 =
        QuillImageFilterFactory::createImageFilter("org.maemo.scale");
    f9->setOption(QuillImageFilter::SizeAfter, QVariant(QSize(3000, 2250)));

    // Load

    QuillImageFilter *f10 =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");
    f10->setOption(QuillImageFilter::FileName, QVariant("/usr/share/quillimagefilter-benchmark/benchmark12.jpg"));

    // Load (thumb load)

    QuillImageFilter *f11 =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");
    f11->setOption(QuillImageFilter::FileName, QVariant("/usr/share/quillimagefilter-benchmark/thumb.jpg"));

    // Save

    QuillImageFilter *f12 =
        QuillImageFilterFactory::createImageFilter("org.maemo.save");
    QTemporaryFile file;
    file.open();
    f12->setOption(QuillImageFilter::FileName, QVariant(file.fileName()));

    // Auto colors
    QuillImageFilter *g1 =
        QuillImageFilterFactory::createImageFilter("org.maemo.auto.colors");

    // Auto contrast
    QuillImageFilter *g2 =
        QuillImageFilterFactory::createImageFilter("org.maemo.auto.contrast");

    // Auto levels
    QuillImageFilter *g3 =
        QuillImageFilterFactory::createImageFilter("org.maemo.auto.levels");

    QuillImageFilter *filters[] = {f1, f2, f3, /*f4,*/ f5, f6, f7, f8, f9, f10, f11, f12,
                                0};

    QuillImageFilter *generators[] = {g1, g2, g3, 0};

    QImage image1("/usr/share/quillimagefilter-benchmark/benchmark12.jpg");

    QSize size[] = {QSize(6000, 4500),
                    QSize(4000, 3000),
                    QSize(3652, 2739),
                    QSize(3266, 2450),
                    QSize(2582, 1937),
                    QSize(2000, 1500),
                    QSize(1600, 1200),
                    QSize(1000, 750),
                    QSize(533, 400),
                    QSize(500, 375),
                    QSize(256, 256),
                    QSize(128, 128),
                    QSize()};

    for (int i = 0; size[i] !=  QSize(); ++i) {
        QuillImage image = image1.scaled(size[i],
                                         Qt::IgnoreAspectRatio,
                                         Qt::SmoothTransformation);

        const int nRepeat = 5;
        int msecs[12][nRepeat];
        for (int repeat = 0; repeat < nRepeat; repeat++) {
            for (int f = 0; 0 != filters[f]; ++f) {
                QSize fullSize;

                // Setting save and scale so that they make sense

                if (filters[f]->role() == QuillImageFilter::Role_Save) {
                    delete filters[f];
                    filters[f] = QuillImageFilterFactory::createImageFilter("org.maemo.save");
                    QTemporaryFile file;
                    file.open();
                    filters[f]->setOption(QuillImageFilter::FileName, QVariant(file.fileName()));
                    fullSize = image.size();
                } else if (filters[f]->role() == QuillImageFilter::Role_PreviewScale) {
                    fullSize = image.size();
                    filters[f]->setOption(QuillImageFilter::SizeAfter,
                                          QVariant(fullSize/2));
                } else
                    fullSize = QSize(6000, 4500);

                image.setFullImageSize(fullSize);
                image.setArea(QRect(0, 0, fullSize.width(), fullSize.height()));

                QTime time;
                time.start();
                filters[f]->apply(image);
                msecs[f][repeat] = time.elapsed();
            }
        }
        for (int f = 0; 0 != filters[f]; ++f) {
            unsigned long _msecs = 0;
            for (int repeat = 0; repeat < nRepeat; repeat++)
                _msecs += msecs[f][repeat];
            _msecs = _msecs / nRepeat;
            qDebug() << "Image" << size[i]
                    << "Filter" << filters[f]->name() << f
                    << "Execution time" << _msecs << "ms";
        }
        qDebug("  -----");

        for(int g = 0; 0 != generators[g]; ++g)
        {
            QTime time;
            time.start();
            QuillImageFilterGenerator *generator =
                dynamic_cast<QuillImageFilterGenerator*>(generators[g]);

            if (generator) {

                generator->apply(image);

                int msecs = time.elapsed();

                qDebug() << "Image" << size[i]
                         << "Generator" << generators[g]->name()
                         << "Generation time" << msecs << "ms";
            } else
                qDebug() << "Error: Generator initialization failed!";
        }
        qDebug("  -----");
    }

    QuillImageFilter *ft =
        QuillImageFilterFactory::createImageFilter("org.maemo.load");
    ft->setOption(QuillImageFilter::FileName, QVariant("/usr/share/quillimagefilter-benchmark/benchmark12.jpg"));

    QuillImage image = QuillImage(QImage(QSize(256, 256), QImage::Format_RGB32));
    image.setFullImageSize(QSize(6000, 4500));
    image.setArea(QRect(2048, 2048, 256, 256));

    QTime time;
    time.start();

    ft->apply(image);

    int time1 = time.elapsed();

    ft->apply(image);

    int time2 = time.elapsed() - time1;

    qDebug() << "Image size" << image.fullImageSize() << "Tile loading" << image.size() << "1st execution" << time1 << "ms";

    qDebug() << "Image size" << image.fullImageSize() << "Tile loading" << image.size() << "2nd execution" << time2 << "ms";

    qDebug("  -----");

    // Red eye detection
    QuillImageFilterGenerator *gr =
        dynamic_cast<QuillImageFilterGenerator*>
        (QuillImageFilterFactory::createImageFilter("org.maemo.red-eye-detection"));

    if (!gr) {
        qDebug() << "Error: Generator initialization failed";
    } else {
        gr->setOption(QuillImageFilter::Center, QVariant(QPoint(108,93)));
        gr->setOption(QuillImageFilter::Radius, QVariant(40));

        time.start();
        QImage grImage = QImage("/usr/share/quillimagefilter-benchmark/redEye.jpg");
        gr->apply(grImage);
        QuillImageFilter *filter = gr->resultingFilter();
        int gmsecs = time.elapsed();

        time.start();
        QImage target = filter->apply(grImage);
        int emsecs = time.elapsed();

        QPolygon areaOfEffect = filter->option(QuillImageFilter::SelectionArea).
            value<QPolygon>();
        int area = 0;
        for (int y=0; y<grImage.width(); y++)
            for (int x=0; x<grImage.width(); x++)
                if (areaOfEffect.contains(QPoint(x, y)))
                    area++;

        qDebug() << "Image" << grImage.size()
                 << "Generator" << gr->name()
                 << "Generation time" << gmsecs << "ms";
        qDebug() << "Pixels affected:" << area << "(reference in initial: 78)";
        qDebug() << "Image" << grImage.size()
                 << "Filter" << filter->name()
                 << "Execution time" << emsecs << "ms";
        qDebug("  -----");
    }
}

