#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <vector>
#include <cstdint>

#include "buffer.h"

// YUV4:2:2
struct YUVPix {
    uint8_t lum;
    uint8_t chrom_u;
    uint8_t chrom_v;
};

struct RGBPix {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

struct RGBImage {

};

namespace Processor {

    YUVPix CalculateAverage(const Buffer& buffer, int width, int height);
    YUVPix CalculateAverage(const Buffer& buffer, int start, int end, int width, int height);

    RGBPix ConvertToRGB(const YUVPix& yuv); 
    
}

#endif // !CALLBACKS_H