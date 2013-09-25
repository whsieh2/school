/**
 * @file image_tools.cpp
 * This file contains the implementation of the image tools; you
 *  will write your code in this file.
 */

#include "image_tools.h"

/**
 * Flips the source image into a new PNG.
 * THIS FUNCTION IS GRADED.
 * @todo - parallelize verticalFlip()
 * @param source - the original PNG
 * @return - a copy of the parameter that has been flipped
 *  over the vertical axis
 */
PNG ImageTools::verticalFlip(const PNG & source)
{
    int width = source.width();
    int height = source.height();
    PNG output(width, height);

    for(int i = 0; i < width; ++i)
    {
        for(int j = 0; j < height; ++j)
        {
            // flip! 
        }
    }

    return output;
}

/**
 * Removes an RGB color component from the PNG.
 * THIS FUNCTION IS GRADED.
 * @todo - parallelize removeColor()
 * @param source - the original PNG
 * @param color - the color to remove
 * @return - a PNG object with an RGB component taken out
 *  of each pixel (if color parameter is valid)
 */
PNG ImageTools::removeColor(const PNG & source, const Color & color)
{
    int width = source.width();
    int height = source.height();
    PNG output(width, height);

    for(int i = 0; i < width; ++i)
    {
        for(int j = 0; j < height; ++j)
        {
            *output(i, j) = *source(i, j);
            switch(color)
            {
                case RED:
                    break;
                case GREEN:
                    break;
                case BLUE:
                    break;
            }
        }
    }

    return output;
}
