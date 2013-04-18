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
#include "pluginloader.h"
#include "quillimagefilterinterface.h"
#include "quillimagefiltergenerator.h"
#include "quillimagefiltergeneratorimplementation.h"

class QuillImageFilterGeneratorPrivate {
public:
    QuillImageFilter *m_result;
    bool m_resultingFilterPassed;
};

QuillImageFilterGenerator::QuillImageFilterGenerator(const QString &name) :
    priv(new QuillImageFilterGeneratorPrivate)
{
    priv->m_result = 0;
    priv->m_resultingFilterPassed = 0;
    setValid(true);

    QuillImageFilterInterface *plugin = PluginLoader::instance(name);
    QuillImageFilterImplementation *handler = 0;
    QuillImageFilterImplementation *impl = 0;
    if (plugin) {
        impl = plugin->create(name);
        handler = dynamic_cast<QuillImageFilterGeneratorImplementation*>(impl);
    }
    if (!handler) {
        delete impl;
        handler = new QuillImageFilterGeneratorImplementation;
        setValid(false);
    }

    setHandler(handler);
}

QuillImageFilterGenerator::~QuillImageFilterGenerator()
{
    if (!priv->m_resultingFilterPassed)
        delete priv->m_result;
    delete priv;
}

QuillImage QuillImageFilterGenerator::apply(const QuillImage &image) const
{
    QuillImageFilterGeneratorImplementation *impl =
        dynamic_cast<QuillImageFilterGeneratorImplementation*>(handler());

    if (impl) {
        priv->m_result = impl->generate(image);
    }
    return QuillImage();
}

QuillImageFilter *QuillImageFilterGenerator::resultingFilter() const
{
    priv->m_resultingFilterPassed = true;
    return priv->m_result;
}

bool QuillImageFilterGenerator::isUsedOnPreview() const
{
    QuillImageFilterGeneratorImplementation *generator =
        dynamic_cast<QuillImageFilterGeneratorImplementation*>(handler());
    if (generator)
        return generator->isUsedOnPreview();
    else
        return true;
}
