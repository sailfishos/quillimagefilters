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

#include <QApplication>
#include <QDir>
#include <QDebug>

#include <QuillImageFilter>
#include <QuillImageFilterFactory>
#include <QuillImageFilterGenerator>

/* Red eye removal quality benchmark */

// 070215161926_N73.jpg : 660 924, 888 918
// 070216104323_N73.jpg : 936 576, 1392 592
// 070216104610_N73.jpg : 1054 544, 1180 562
// 070216150537_N73.jpg : 1064 850, 1190 850
// 070216164250_N73.jpg : 894 652, 1078 660
// 070218201220_N73.jpg : 1184 466, 1462 444
// 070218201838_N73.jpg : 632 610, 828 614, 1310 592, 1528 562
// 070219134310_N73.jpg : 980 350, 1162 350
// Image_0_47125_4.jpg : 1248 690, 1434 708
// Image_0_63875_4.jpg : 738 714, 882 714, 1782 597, 1938 603
// Image_0_83125_4.jpg : 714 813, 819 792, 1701 798, 1824 804
// Image_0_110875_4.jpg : 1281 1050, 1356 1059
// Image_0_128750_4.jpg : 1185 837, 1413 768
// Image_0_131250_4.jpg : 1356 804, 1608 852
// Image_0_170625_4.jpg : 1182 930, 1278 903
// Image_0_183625_4.jpg : 1098 1017, 1527 1023
// Image_0_453750_4.jpg : 1257 816, 1473 804
// Image_0_476000_4.jpg : 822 768, 951 762, 1974 1050, 2109 1044
// Image_0_483000_4.jpg : 1180 993, 1350 980
// Image_0_761750_4.jpg : 1311 999, 1539 984

QString fileNames[20] = {
    "070215161926_N73.jpg",
    "070216104323_N73.jpg",
    "070216104610_N73.jpg",
    "070216150537_N73.jpg",
    "070216164250_N73.jpg",
    "070218201220_N73.jpg",
    "070218201838_N73.jpg",
    "070219134310_N73.jpg",
    "Image_0_47125_4.jpg",
    "Image_0_63875_4.jpg",
    "Image_0_83125_4.jpg",
    "Image_0_110875_4.jpg",
    "Image_0_128750_4.jpg",
    "Image_0_131250_4.jpg",
    "Image_0_170625_4.jpg",
    "Image_0_183625_4.jpg",
    "Image_0_453750_4.jpg",
    "Image_0_476000_4.jpg",
    "Image_0_483000_4.jpg",
    "Image_0_761750_4.jpg"
};

QPoint redEyes[20][4] = {
    {QPoint(660, 924), QPoint(888, 918), QPoint(), QPoint()},
    {QPoint(936, 576), QPoint(1392, 592), QPoint(), QPoint()},
    {QPoint(1054, 544), QPoint(1180, 562), QPoint(), QPoint()},
    {QPoint(1064, 850), QPoint(1190, 850), QPoint(), QPoint()},
    {QPoint(894, 652), QPoint(1078, 660), QPoint(), QPoint()},
    {QPoint(1184, 466), QPoint(1462, 444), QPoint(), QPoint()},
    {QPoint(632, 610), QPoint(828, 614), QPoint(1318, 592), QPoint(1528, 562)},
    {QPoint(980, 350), QPoint(1162, 350), QPoint(), QPoint()},
    {QPoint(1248, 690), QPoint(1434, 708), QPoint(), QPoint()},
    {QPoint(738, 714), QPoint(882, 714), QPoint(1782, 597), QPoint(1938, 603)},
    {QPoint(714, 813), QPoint(819, 792), QPoint(1701, 798), QPoint(1824, 804)},
    {QPoint(1281, 1050), QPoint(1356, 1059), QPoint(), QPoint()},
    {QPoint(1185, 837), QPoint(1413, 768), QPoint(), QPoint()},
    {QPoint(1356, 804), QPoint(1608, 852), QPoint(), QPoint()},
    {QPoint(1182, 930), QPoint(1278, 903), QPoint(), QPoint()},
    {QPoint(1098, 1017), QPoint(1527, 1023), QPoint(), QPoint()},
    {QPoint(1257, 816), QPoint(1473, 804), QPoint(), QPoint()},
    {QPoint(882, 768), QPoint(951, 762), QPoint(1974, 1050), QPoint(2109, 1044)},
    {QPoint(1180, 993), QPoint(1350, 980), QPoint(), QPoint()},
    {QPoint(1311, 999), QPoint(1539, 984), QPoint(), QPoint()}
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QuillImageFilter::registerAll();

    QuillImageFilterGenerator *filter =
        dynamic_cast<QuillImageFilterGenerator*>
        (QuillImageFilterFactory::createImageFilter("RedEyeDetection"));

    QDir dir = QDir::current();
    dir.mkdir("results");

    for (int i=0; i<20; i++)
    {
        QImage qImage(fileNames[i]);
        QuillImage image(qImage);
        qDebug() << "applying to image" << fileNames[i] << "size" << image.size();
        for (int j=0; j<4; j++)
            if (redEyes[i][j] != QPoint())
            {
                qDebug() << "trying to remove red eye at" << redEyes[i][j];
                filter->setOption(QuillImageFilter::Center, redEyes[i][j]);
                filter->setOption(QuillImageFilter::Radius, QVariant(40));
                filter->apply(image);
                image = filter->resultingFilter()->apply(image);
            }
        qDebug() << image.size();

        image.save("results/quill_" + fileNames[i]);
    }
}
