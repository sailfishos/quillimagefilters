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

/*!
  \mainpage Quill Image Filters Reference Documentation

  \section intro_sec Introduction

Quill Image Filters is a library containing different image editing
operations "filters". Their primary use is by the image editing
library LibQuill, but these image editing operations, targeting better
efficiency than their Qt counterparts, can be used by other
applications as well.

  \section overview_sec Overview

The image filters operate using the QuillImage class, which is an
extension to the QImage class. In addition to full-resolution images,
QuillImages can represent only parts of an image "tiles", or
downgraded-resolution representation of the images
"previews/thumbnails". QuillImageFilters treat their input images
accordingly: for example, the Crop filter applied to a tile completely
within the crop region will have no effect.
*/

/*!
  \class QuillImageFilter

  \brief Represents one image editing operation.

Image editing operations operate with instances of QuillImage class.
They are used by calling apply().

To keep the public API minimal, individual filter constructors are not
in the public interface. Instead, Quill image filters are generated
for the application by the QuillImageFilterFactory class.

Filters can be parametrized using options, see setOption() and
option(); and QuillFilterOption for the list of available
options. Some filters like BrightnessContrast or RedEyeReduction
always require some options to be set; others like Crop also have
default values.
*/

#ifndef __QUILL_IMAGE_FILTER_GENERIC_H__
#define __QUILL_IMAGE_FILTER_GENERIC_H__

#include <QVariant>
#include <QLatin1String>
#include <QStringList>

#include "quillimage.h"

class QIODevice;
class QuillImageFilterInterface;
class QuillImageFilterImplementation;
class QuillImageFilterPrivate;

class QuillImageFilter
{
public:
    typedef QString QuillFilterOption;

    static const QLatin1String FileName;
    static const QLatin1String FileFormat;
    static const QLatin1String Timestamp;

    static const QLatin1String Name_AutoContrast;
    static const QLatin1String Name_BrightnessContrast;
    static const QLatin1String Name_Crop;
    static const QLatin1String Name_Load;
    static const QLatin1String Name_Overlay;
    static const QLatin1String Name_Save;
    static const QLatin1String Name_ScaleCrop;
    static const QLatin1String Name_Flip;
    static const QLatin1String Name_FreeRotate;
    static const QLatin1String Name_Gaussian;
    static const QLatin1String Name_Lanczos;
    static const QLatin1String Name_Levels;
    static const QLatin1String Name_AutoLevels;
    static const QLatin1String Name_RedEyeReduction;
    static const QLatin1String Name_RedEyeDetection;
    static const QLatin1String Name_Rotate;
    static const QLatin1String Name_Scale;

    static const QLatin1String Name_Greyscale;
    static const QLatin1String Name_Sepia;
    static const QLatin1String Name_OldColorPhoto;
    static const QLatin1String Name_CrossProcessing;

    static const QLatin1String Name_AutoContrast_deprecated;
    static const QLatin1String Name_BrightnessContrast_deprecated;
    static const QLatin1String Name_Crop_deprecated;
    static const QLatin1String Name_Load_deprecated;
    static const QLatin1String Name_Overlay_deprecated;
    static const QLatin1String Name_Save_deprecated;
    static const QLatin1String Name_ScaleCrop_deprecated;
    static const QLatin1String Name_Flip_deprecated;
    static const QLatin1String Name_FreeRotate_deprecated;
    static const QLatin1String Name_Gaussian_deprecated;
    static const QLatin1String Name_Lanczos_deprecated;
    static const QLatin1String Name_Levels_deprecated;
    static const QLatin1String Name_AutoLevels_deprecated;
    static const QLatin1String Name_RedEyeReduction_deprecated;
    static const QLatin1String Name_RedEyeDetection_deprecated;
    static const QLatin1String Name_Rotate_deprecated;
    static const QLatin1String Name_Scale_deprecated;


    /*!
      filter has permanent effects and therefore should not be undone.
    */
    static const QLatin1String DisableUndo;
    /*!
      filter should not be applied to preview image of any resolution.
    */
    static const QLatin1String DisablePreview;
    /*!
      to specify new full image size.
    */
    static const QLatin1String SizeAfter;
    /*!
      Brightness component of the brightness/contrast filter
      (percentage units)
    */
    static const QLatin1String Brightness;
    /*!
      Contrast component of the brightness/contrast filter
      (percentage units)
    */
    static const QLatin1String Contrast;
    /*!
      Minimum red level (0-255)
    */
    static const QLatin1String Red;
    /*!
      Minimum green level (0-255)
    */
    static const QLatin1String Green;
    /*!
      Minimum blue level (0-255)
    */
    static const QLatin1String Blue;
    /*!
      Maximum red level (0-255)
    */
    static const QLatin1String RedMax;
    /*!
      Maximum green level (0-255)
    */
    static const QLatin1String GreenMax;
    /*!
      Maximum blue level (0-255)
    */
    static const QLatin1String BlueMax;
    /*!
      used for rotations
    */
    static const QLatin1String Angle;
    /*!
      the rectangle used to crop image
    */
    static const QLatin1String CropRectangle;
    /*!
      The centerpoint of red eye detection area
    */
    static const QLatin1String Center;
    /*!
      Radius of effect (eye radius): for red eye detection, should be an integer
    */
    static const QLatin1String Radius;
    /*!
      Tap error tolerance: for red eye detection, should be an integer
    */
    static const QLatin1String Tolerance;
    /*!
      If the flip effect is horizontal or not
    */
    static const QLatin1String Horizontal;
    /*!
      For the overlay filter since it needs to process two images.
    */
    static const QLatin1String Background;
    /*!
      Total tile count for save filter
    */
    static const QLatin1String TileCount;
    /*!
      Selection area (for red eye reduction, should be a QPolygon)
    */
    static const QLatin1String SelectionArea;
    /*!
      Raw EXIF data dumped into a byte array
    */
    static const QLatin1String RawExifData;
    /*!
      MIME type of the image to be loaded or saved
    */
    static const QLatin1String MimeType;
    /*!
      Background color for alpha rendering
    */
    static const QLatin1String BackgroundColor;
    /*!
      Boolean check if the load filter should ignore EXIF orientation
    */
    static const QLatin1String IgnoreExifOrientation;


    enum QuillFilterRole {
        /*!
        Used for enumerating through the filter roles
        */

        /*!
        By default, filters do not have a role
        */
        Role_Undefined = 0,
        /*!
        Filter role for loading an image
        */
        Role_Load,
        /*!
        Filter role for saving an image
        */
        Role_Save,
        /*!
        Overlay save helper - knows how fill the saving buffer
        */
        Role_Overlay,
        /*!
        Filter role to provide a preview scaling
        */
        Role_PreviewScale
    };

    enum ImageFilterError{
        UnspecifiedError = -1,
        NoError = 0,
        FileNotFoundError,
        UnsupportedFormatError,
        InvalidDataError,
        FileCanNotOpenError,
        CanceledError
    };

public:

    /*!
      Initializes an invalid image filter.
     */

    QuillImageFilter();

    /*!

      Initializes an image filter with a given name. If a filter
      corresponding to the name is not found from within the plugin database,
     */

    QuillImageFilter(const QString &name);

    /*!

      Initializes an image filter with a given role. If no filter implementing
      the given role is foung, an invalid filter is created instead.
     */

    QuillImageFilter(QuillFilterRole role);

    /*!
      Frees common things to all filters.
     */

    virtual ~QuillImageFilter();

    /*!
      Check if a filter is valid (if a filter was found from any plugin).
     */

    bool isValid();

    /*!
      Returns the value of a filter option. These options are the
      recommended way to pass information to the filters. This
      function needs to be reimplemented in a derived class to support
      options.
     */

    QVariant option(const QString &filterOption) const;

    /*!
      Sets the value of a filter option. These options are the recommended
      way to pass information to the filters. This
      function needs to be reimplemented in a derived class to support
      options.
     */

    bool setOption(const QString &filterOption, const QVariant &value);

    /*!
      Returns true if a filter supports the given option. This
      function needs to be reimplemented in a derived class to support
      options.
     */

    bool supportsOption(const QString &filterOption) const;

    /*!
      Returns the list of all options supported by the filter.
     */

    const QStringList supportedOptions() const;

    /*!
      Applies the filter to a given preview or fragment.
     */

    virtual QuillImage apply(const QuillImage &image) const;

    /*!
      If the filter changes dimensions of the image,
      this can be used to re-calculate a new size for the full image.
     */

    QSize newFullImageSize(const QSize &fullImageSize) const;

    /*!
      If the filter changes dimensions of the image,
      this can be used to re-calculate a new position for a specific tile.
    */

    QRect newArea(const QSize &fullImageSize, const QRect &area) const;

    /*!
      Returns the name of the filter.
    */
    const QString name() const;

    /*!
      Returns the role of this filter.

      The role system is used by LibQuill when it auto-generates
      filters for its own use. This will also allow later registering
      third party filters to replace LibQuill defaults.
    */
    QuillFilterRole role() const;

    /*!
      Returns the IO device used, see setIODevice().
     */
    QIODevice* iODevice() const;

    /*!
      Sets the IO device used. This is only used by filters which
      represent file system access (loading and saving). The device
      remains property of the caller but remains in use by the filter;
      it should only be deleted after the filter has been removed.
    */
    void setIODevice(QIODevice* iODevice);

    /*!
      Note: This is currently an API placeholder with no actual
      functionality.

      Cancel a call to apply() which is currently running in some
      other thread. This is the only method of its class which is
      thread safe. Cancellation implementation by filter is
      optional. When the canceled filter recognizes the cancellation,
      it will terminate gracefully, return an invalid QuillImage, and
      have its error() value set as QuillImageFilter::CanceledError.
    */
    void cancel();

    /*!
     */
    QuillImageFilter::ImageFilterError error() const;

    /*!
      Clean up allocated but otherwise unused resources. This will
      also call for cleanup in all loaded plugins.

      \warning This function is <b>not thread safe</b>. It must not
         be called while filter plugins are running. When using
         libquill, it is recommended to do cleanup using the
         <tt>Quill::cleanup()</tt>, as it will also call this
         function safely.
     */
    static void cleanup();

 protected:
    QuillImageFilterImplementation *handler() const;

    void setHandler(QuillImageFilterImplementation *handler);

    void setValid(bool valid);

 private:
    QuillImageFilterPrivate *priv;
};


#endif // __QUILL_IMAGE_FILTER_GENERIC_H__
