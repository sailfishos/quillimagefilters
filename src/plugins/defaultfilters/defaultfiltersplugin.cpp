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

#include "../base/quillimagefilter.h"
#include "../base/quillimagefilterinterface.h"
#include "loadfilter.h"
#include "overlay.h"
#include "savefilter.h"
#include "scalecrop.h"
#include "serialsaver.h"
#include "defaultfiltersplugin.h"

DefaultFiltersPlugin::DefaultFiltersPlugin()
{
}

QuillImageFilterImplementation *DefaultFiltersPlugin::create(const QString &name)
{
    if ((name == QuillImageFilter::Name_Load)
        | (name == QuillImageFilter::Name_Load_deprecated))
        return new LoadFilter;
    else if ((name == QuillImageFilter::Name_Overlay)
             | (name == QuillImageFilter::Name_Overlay_deprecated))
        return new Overlay;
    else if ((name == QuillImageFilter::Name_Save)
             | (name == QuillImageFilter::Name_Save_deprecated))
        return new SaveFilter;
    else if ((name == QuillImageFilter::Name_ScaleCrop)
             | (name == QuillImageFilter::Name_ScaleCrop_deprecated))
        return new ScaleCrop;
    else
        return 0;
}

QuillImageFilterImplementation *DefaultFiltersPlugin::create(QuillImageFilter::QuillFilterRole role)
{
    switch (role) {
        case QuillImageFilter::Role_PreviewScale: return new ScaleCrop;
        case QuillImageFilter::Role_Overlay: return new Overlay;
        case QuillImageFilter::Role_Load: return new LoadFilter;
        case QuillImageFilter::Role_Save: return new SaveFilter;
        default: return 0;
    }
}

const QStringList DefaultFiltersPlugin::name() const
{
    QStringList list;
    list.append(QuillImageFilter::Name_Load);
    list.append(QuillImageFilter::Name_Overlay);
    list.append(QuillImageFilter::Name_Save);
    list.append(QuillImageFilter::Name_ScaleCrop);
    list.append(QuillImageFilter::Name_Load_deprecated);
    list.append(QuillImageFilter::Name_Overlay_deprecated);
    list.append(QuillImageFilter::Name_Save_deprecated);
    list.append(QuillImageFilter::Name_ScaleCrop_deprecated);
    return list;
}

const QList<QuillImageFilter::QuillFilterRole> DefaultFiltersPlugin::role() const
{
    QList<QuillImageFilter::QuillFilterRole> list;
    list.append(QuillImageFilter::Role_Overlay);
    list.append(QuillImageFilter::Role_Load);
    list.append(QuillImageFilter::Role_Save);
    list.append(QuillImageFilter::Role_PreviewScale);
    return list;
}

void quillimagefilter_plugin_cleanup()
{
    LoadFilter::cleanup();
}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(defaultfilters, DefaultFiltersPlugin)
#endif
