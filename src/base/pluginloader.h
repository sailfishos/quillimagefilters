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

#ifndef __PLUGIN_LOADER_H__
#define __PLUGIN_LOADER_H__

#include "quillimagefilterinterface.h"
#include <QString>

class PluginLoader
{
 public:
    static QuillImageFilterInterface *instance(const QString &name);
    static QuillImageFilterInterface *instance(QuillImageFilter::QuillFilterRole role);
    static void cleanup();

 private:
    static void update(const QString &name);
    static void update(const QuillImageFilter::QuillFilterRole role);
    static void searchLibrary(const QuillImageFilter::QuillFilterRole role);
    static void searchLibrary(const QString &name);
    static void loadLibrary(const QString &name);
    static void findLibraryPath();

 private:
    typedef void (*CleanupFunction)();

    static QMap<QString, QuillImageFilterInterface*> m_plugins;
    static QMap<QuillImageFilter::QuillFilterRole, QuillImageFilterInterface*> m_pluginsByRole;
    static QString filterRole;
    static QStringList paths;
    static QStringList pluginAbsolutePaths;
    static QStringList loadedAbsolutePaths;
    static QList<CleanupFunction> m_cleanupFunctions;
    static const QLatin1String defaultFilterName;
    static const QLatin1String quillImageFilterPluginDir;

};

#endif //__PLUGIN_LOADER_H__
