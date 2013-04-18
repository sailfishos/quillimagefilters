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
    QCoreApplication::addLibraryPath("/usr/local/plugins");
    // Load

    QuillImageFilter *f10 =
        QuillImageFilterFactory::createImageFilter("Load");
    f10->setOption(QuillImageFilter::FileName, QVariant("/usr/share/quillimagefilter-benchmark/benchmark12.jpg"));
    // Load (Qt performance bug corrected)
    QuillImageFilter *filters[] = {f10,0};
    QImage image1("/usr/share/quillimagefilter-benchmark/benchmark12.jpg");
    QSize size[] = {QSize(4000, 3000),QSize()};
    for (int i = 0; size[i] !=  QSize(); ++i) {
        for (int f = 0; 0 != filters[f]; ++f) {
            QTime time;
            time.start();
            filters[f]->apply(image1);
            int msecs = time.elapsed();
            qDebug() << "Image" << size[i]
                     << "Filter" << filters[f]->name()
                     << "Execution time" << msecs << "ms";
        }
        qDebug("  -----");
    }

    QuillImageFilter *ft =
        QuillImageFilterFactory::createImageFilter("Load");
    ft->setOption(QuillImageFilter::FileName, QVariant("/usr/share/quillimagefilter-benchmark/benchmark12.jpg"));

    QuillImage image = QuillImage(QImage(QSize(256, 256), QImage::Format_RGB32));
    image.setFullImageSize(QSize(4000, 3000));
    image.setArea(QRect(2048, 2048, 256, 256));
    ft->apply(image);

}

