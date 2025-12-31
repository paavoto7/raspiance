#include "processor.h"

// 	Pixel Format : 'YUYV' (YUYV 4:2:2)

namespace Processor {

    YUVPix CalculateAverage(const Buffer& buffer, int width, int height) {

        uint64_t lum = 0;
        uint64_t chrom_u = 0;
        uint64_t chrom_v = 0;
        
        const auto size = buffer.size;
        for (int i = 0; i < size; i += 4) {
            
            lum += buffer.data[i] + buffer.data[i + 2];
            chrom_u += buffer.data[i + 1];
            chrom_v += buffer.data[i + 3];
        }

        const uint64_t l = width * height;
        return { lum / l, chrom_u / (l / 2), chrom_v / (l / 2) };
    }

    YUVPix CalculateAverage(const Buffer& buffer, int start, int end, int width, int height) {

        for (int i = 0; i < buffer.size; i += 4) {
            
        }
        
        return { };
    }

    RGBPix ConvertToRGB(const YUVPix& yuv) {
        return { };
    }
    
}
