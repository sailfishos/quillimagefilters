#include "quillimage.h"
#include "vignette.h"
#include <cmath>

QuillImage Vignette::apply(QuillImage image, float radius, float strength)
{
    // full image size in image coordinates
    int fullX = image.width() * image.fullImageSize().width() /
        image.area().width();
    int fullY = image.height() * image.fullImageSize().height() /
        image.area().height();

    // centerpoint of image, from full-image coordinates to image coordinates
    int scaledLeft = image.area().left() * image.width() / image.area().width();
    int centerX = fullX/2 - scaledLeft;
    int scaledTop = image.area().top() * image.height() / image.area().height();
    int centerY = fullY/2 - scaledTop;

    int vignetteEffectMaxRadius = sqrt((fullX*fullX+fullY*fullY)/4);
    int vignetteEffectMinRadius = vignetteEffectMaxRadius*(1-radius);

    int vignetteEffectRadiusDiff = vignetteEffectMaxRadius-
        vignetteEffectMinRadius;

    QRgb *p = (QRgb*)image.bits();

    for (int y=0; y<image.height(); y++) {
        int yDist2 = (y - centerY) * (y - centerY);

        for (int x=0; x<image.width(); x++) {
            int xDist = x - centerX;
            float dist = sqrt(xDist*xDist + yDist2);

            if (dist > vignetteEffectMinRadius) {
                float effectDist = (dist-vignetteEffectMinRadius)/
                    (vignetteEffectRadiusDiff);

                float effect = 1 - effectDist * effectDist * strength;

                QRgb rgb = *p;
                int red = qRed(rgb) * effect;
                int green = qGreen(rgb) * effect;
                int blue = qBlue(rgb) * effect;

                *p = qRgba(red, green, blue, qAlpha(rgb));
            }

            p++;
        }
    }

    return image;
}
