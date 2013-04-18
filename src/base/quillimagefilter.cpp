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

#include <QVariant>
#include <QIODevice>
#include "quillimagefilter.h"
#include "pluginloader.h"
#include "quillimagefilterinterface.h"
#include "quillimagefilterimplementation.h"


const QLatin1String QuillImageFilter::Name_AutoContrast_deprecated("org.maemo.auto.contrast");
const QLatin1String QuillImageFilter::Name_BrightnessContrast_deprecated("org.maemo.composite.brightness.contrast");
const QLatin1String QuillImageFilter::Name_Crop_deprecated("org.maemo.crop");
const QLatin1String QuillImageFilter::Name_Load_deprecated("org.maemo.load");
const QLatin1String QuillImageFilter::Name_Overlay_deprecated("org.maemo.overlay");
const QLatin1String QuillImageFilter::Name_Save_deprecated("org.maemo.save");
const QLatin1String QuillImageFilter::Name_ScaleCrop_deprecated("org.maemo.composite.scale.crop");
const QLatin1String QuillImageFilter::Name_Flip_deprecated("org.maemo.flip");
const QLatin1String QuillImageFilter::Name_FreeRotate_deprecated("org.maemo.free-rotate");
const QLatin1String QuillImageFilter::Name_Gaussian_deprecated("org.maemo.blur.gaussian");
const QLatin1String QuillImageFilter::Name_Lanczos_deprecated("org.maemo.scale.lanczos");
const QLatin1String QuillImageFilter::Name_Levels_deprecated("org.maemo.levels");
const QLatin1String QuillImageFilter::Name_AutoLevels_deprecated("org.maemo.auto.levels");
const QLatin1String QuillImageFilter::Name_RedEyeReduction_deprecated("org.maemo.red-eye-reduction");
const QLatin1String QuillImageFilter::Name_RedEyeDetection_deprecated("org.maemo.red-eye-detection");
const QLatin1String QuillImageFilter::Name_Rotate_deprecated("org.maemo.rotate");
const QLatin1String QuillImageFilter::Name_Scale_deprecated("org.maemo.scale");

const QLatin1String QuillImageFilter::Name_AutoContrast("com.meego.auto.contrast");
const QLatin1String QuillImageFilter::Name_BrightnessContrast("com.meego.composite.brightness.contrast");
const QLatin1String QuillImageFilter::Name_Crop("com.meego.crop");
const QLatin1String QuillImageFilter::Name_Load("com.meego.load");
const QLatin1String QuillImageFilter::Name_Overlay("com.meego.overlay");
const QLatin1String QuillImageFilter::Name_Save("com.meego.save");
const QLatin1String QuillImageFilter::Name_ScaleCrop("com.meego.composite.scale.crop");
const QLatin1String QuillImageFilter::Name_Flip("com.meego.flip");
const QLatin1String QuillImageFilter::Name_FreeRotate("com.meego.free-rotate");
const QLatin1String QuillImageFilter::Name_Gaussian("com.meego.blur.gaussian");
const QLatin1String QuillImageFilter::Name_Lanczos("com.meego.scale.lanczos");
const QLatin1String QuillImageFilter::Name_Levels("com.meego.levels");
const QLatin1String QuillImageFilter::Name_AutoLevels("com.meego.auto.levels");
const QLatin1String QuillImageFilter::Name_RedEyeReduction("com.meego.red-eye-reduction");
const QLatin1String QuillImageFilter::Name_RedEyeDetection("com.meego.red-eye-detection");
const QLatin1String QuillImageFilter::Name_Rotate("com.meego.rotate");
const QLatin1String QuillImageFilter::Name_Scale("com.meego.scale");

const QLatin1String QuillImageFilter::Name_Greyscale("com.meego.greyscale");
const QLatin1String QuillImageFilter::Name_Sepia("com.meego.sepia");
const QLatin1String QuillImageFilter::Name_OldColorPhoto("com.meego.old-color-photo");
const QLatin1String QuillImageFilter::Name_CrossProcessing("com.meego.cross-processing");

const QLatin1String QuillImageFilter::FileName("file-name");
const QLatin1String QuillImageFilter::FileFormat("file-format");
const QLatin1String QuillImageFilter::Timestamp("timestamp");
const QLatin1String QuillImageFilter::DisableUndo("disable-undo");
const QLatin1String QuillImageFilter::DisablePreview("disable-preview");
const QLatin1String QuillImageFilter::SizeAfter("target-size");
const QLatin1String QuillImageFilter::Brightness("brightness-component");
const QLatin1String QuillImageFilter::Contrast("contrast-component");
const QLatin1String QuillImageFilter::Red("red-min");
const QLatin1String QuillImageFilter::Green("green-min");
const QLatin1String QuillImageFilter::Blue("blue-min");
const QLatin1String QuillImageFilter::RedMax("red-max");
const QLatin1String QuillImageFilter::GreenMax("green-max");
const QLatin1String QuillImageFilter::BlueMax("blue-max");
const QLatin1String QuillImageFilter::Angle("angle");
const QLatin1String QuillImageFilter::CropRectangle("crop-rectangle");
const QLatin1String QuillImageFilter::Center("center");
const QLatin1String QuillImageFilter::Radius("radius");
const QLatin1String QuillImageFilter::Tolerance("tolerance");
const QLatin1String QuillImageFilter::Horizontal("horizontal");
const QLatin1String QuillImageFilter::Background("background");
const QLatin1String QuillImageFilter::TileCount("tile-count");
const QLatin1String QuillImageFilter::SelectionArea("selection-area");
const QLatin1String QuillImageFilter::RawExifData("exif-block");
const QLatin1String QuillImageFilter::MimeType("mime-type");
const QLatin1String QuillImageFilter::BackgroundColor("background-color");
const QLatin1String QuillImageFilter::IgnoreExifOrientation("ignore-exif-orientation");

class QuillImageFilterPrivate {
public:
    QuillImageFilterImplementation *m_handler;
    bool m_isValid;
};

QuillImageFilter::QuillImageFilter() :
    priv(new QuillImageFilterPrivate)
{
    priv->m_handler = new QuillImageFilterImplementation;
    priv->m_isValid = false;
}

QuillImageFilter::QuillImageFilter(const QString &name) :
    priv(new QuillImageFilterPrivate)
{
    priv->m_handler = 0;
    priv->m_isValid = true;
    QuillImageFilterInterface *plugin = PluginLoader::instance(name);
    if (plugin)
        priv->m_handler = plugin->create(name);

    if (!priv->m_handler) {
        priv->m_handler = new QuillImageFilterImplementation;
        priv->m_isValid = false;
    }
}

QuillImageFilter::QuillImageFilter(QuillFilterRole role):
    priv(new QuillImageFilterPrivate)
{
    priv->m_handler = 0;
    priv->m_isValid = true;

    QuillImageFilterInterface *plugin = PluginLoader::instance(role);
    if (plugin)
        priv->m_handler = plugin->create(role);

    if (!priv->m_handler) {
        priv->m_handler = new QuillImageFilterImplementation;
        priv->m_isValid = false;
    }
}

QuillImageFilter::~QuillImageFilter()
{
    delete priv->m_handler;
    delete priv;
}

bool QuillImageFilter::isValid()
{
    return priv->m_isValid;
}

void QuillImageFilter::setValid(bool valid)
{
    priv->m_isValid = valid;
}

QSize QuillImageFilter::newFullImageSize(const QSize &fullImageSize) const
{
    return priv->m_handler->newFullImageSize(fullImageSize);
}

QRect QuillImageFilter::newArea(const QSize &fullImageSize, const QRect &area) const
{
    return priv->m_handler->newArea(fullImageSize, area);
}

QuillImage QuillImageFilter::apply(const QuillImage &image) const
{
    return priv->m_handler->apply(image);
}

QVariant QuillImageFilter::option(const QString &filterOption) const
{
    return priv->m_handler->option(filterOption);
}

bool QuillImageFilter::setOption(const QString &filterOption, const QVariant &value)
{
    return priv->m_handler->setOption(filterOption, value);
}

bool QuillImageFilter::supportsOption(const QString &filterOption) const
{
    return supportedOptions().contains(filterOption);
}

const QStringList QuillImageFilter::supportedOptions() const
{
    return priv->m_handler->supportedOptions();
}

QuillImageFilter::QuillFilterRole QuillImageFilter::role() const
{
    return priv->m_handler->role();
}

QIODevice* QuillImageFilter::iODevice() const
{
    return priv->m_handler->iODevice();
}

void QuillImageFilter::setIODevice(QIODevice* iODevice)
{
    priv->m_handler->setIODevice(iODevice);
}

QuillImageFilter::ImageFilterError QuillImageFilter::error() const
{
    return priv->m_handler->error();
}

void QuillImageFilter::cancel()
{
    priv->m_handler->cancel();
}

const QString QuillImageFilter::name() const
{
    return priv->m_handler->name();
}

QuillImageFilterImplementation *QuillImageFilter::handler() const
{
    return priv->m_handler;
}

void QuillImageFilter::setHandler(QuillImageFilterImplementation *handler)
{
    delete priv->m_handler;
    priv->m_handler = handler;
}

void QuillImageFilter::cleanup()
{
    PluginLoader::cleanup();
}
