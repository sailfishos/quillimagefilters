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
#include <QDir>
#include <QPluginLoader>
#include "quillimagefilterinterface.h"
#include "quillimagefilter.h"
#include "pluginloader.h"

QStringList PluginLoader::loadedAbsolutePaths = QStringList();
QStringList PluginLoader::paths = QStringList();
QStringList PluginLoader::pluginAbsolutePaths = QStringList();
QMap<QString, QuillImageFilterInterface*> PluginLoader::m_plugins;
QMap<QuillImageFilter::QuillFilterRole, QuillImageFilterInterface*> PluginLoader::m_pluginsByRole;
QList<PluginLoader::CleanupFunction> PluginLoader::m_cleanupFunctions;
const QLatin1String PluginLoader::quillImageFilterPluginDir("/quillimagefilters");
const QLatin1String PluginLoader::defaultFilterName("libdefaultfilters.so");

void PluginLoader::update(const QuillImageFilter::QuillFilterRole role)
{
    if(!m_pluginsByRole.contains(role)){
        searchLibrary(role);
    }
}
void PluginLoader::update(const QString &name)
{
    if(!m_plugins.contains(name)){
        searchLibrary(name);
    }
}

QuillImageFilterInterface *PluginLoader::instance(const QString &name)
{
    update(name);

    return m_plugins.value(name);
}

QuillImageFilterInterface* PluginLoader::instance(QuillImageFilter::QuillFilterRole role)
{
    update(role);

    return m_pluginsByRole.value(role);
}

void PluginLoader::searchLibrary(const QString &name)
{
    findLibraryPath();

    foreach(QString pluginAbsolutePath, pluginAbsolutePaths) {
        if(!m_plugins.contains(name)){
            loadLibrary(pluginAbsolutePath);
        }
    }
}

void PluginLoader::searchLibrary(const QuillImageFilter::QuillFilterRole role)
{
    findLibraryPath();
    foreach(QString pluginAbsolutePath, pluginAbsolutePaths) {
        if(pluginAbsolutePath.endsWith(defaultFilterName)){
            if(!m_pluginsByRole.contains(role)){
                loadLibrary(pluginAbsolutePath);
                break;
            }
        }
    }
}

void PluginLoader::loadLibrary(const QString &name)
{
    if(!loadedAbsolutePaths.contains(name)){
        QPluginLoader loader(name);
        QObject *instance = loader.instance();

        if (!instance) {
            return;
        }
        QuillImageFilterInterface *plugin =
            qobject_cast<QuillImageFilterInterface*>(instance);
        if (!plugin) {
            delete instance;
            return;
        }
        foreach (QuillImageFilter::QuillFilterRole role, plugin->role())
        m_pluginsByRole.insert(role, plugin);

        foreach (QString name, plugin->name())
            m_plugins.insert(name, plugin);
        loadedAbsolutePaths<<name;

        CleanupFunction func = reinterpret_cast<CleanupFunction>(
            QLibrary::resolve(name, "quillimagefilter_plugin_cleanup"));
        if (func && !m_cleanupFunctions.contains(func)) {
            m_cleanupFunctions.append(func);
        }
    }
}

void PluginLoader::findLibraryPath()
{
    if(paths.isEmpty()){
        paths = QCoreApplication::libraryPaths();
        foreach(QString path, paths){
            QStringList pluginNames = QDir(path + quillImageFilterPluginDir).entryList(QDir::Files);
            foreach(QString pluginName, pluginNames) {
                pluginAbsolutePaths<<QDir::cleanPath(path + quillImageFilterPluginDir + QLatin1Char('/') + pluginName);
            }
        }
    }
}

void PluginLoader::cleanup()
{
    // Call all cleanup functions
    foreach (CleanupFunction function, m_cleanupFunctions) {
        (*function)();
    }
}
