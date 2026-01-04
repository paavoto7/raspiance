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

// Defines a rectangular segment in the image
struct Segment {
    int xi, xj;
    int yi, yj;

    // Average of the YUV values that CalculateAverage sets
    YUVPix avg;

    Segment(int xi, int xj, int yi, int yj)
        : xi(xi), xj(xj), yi(yi), yj(yj) {}
    
    inline bool isInBounds(int width, int height) {
        return xi >= 0 && yi >= 0 && xj <= width && yj <= height;
    }
};

// Includes functions for different processing operations of the raw image data
namespace Processor {

    void CalculateAverage(const Buffer& buffer, YUVPix& yuvpix, int width, int height);
    void CalculateAverage(const Buffer& buffer, std::vector<Segment>& segments, int width, int height);

    RGBPix ConvertToRGB(const YUVPix& yuv);
    
}

#endif // !CALLBACKS_H
