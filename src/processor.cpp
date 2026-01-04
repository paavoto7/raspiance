#include "processor.h"

// 	Pixel Format : 'YUYV' (YUYV 4:2:2)

namespace Processor {

    YUVPix CalculateAverage(const Buffer& buffer, int width, int height) {

        uint64_t lum = 0;
        uint64_t chrom_u = 0;
        uint64_t chrom_v = 0;
        
        const auto size = buffer.size;
        for (std::size_t i = 0; i < size; i += 4) {
            
            lum += buffer.data[i] + buffer.data[i + 2];
            chrom_u += buffer.data[i + 1];
            chrom_v += buffer.data[i + 3];
        }

        const uint64_t l = width * height;
        return { lum / l, chrom_u / (l / 2), chrom_v / (l / 2) };
    }

    YUVPix CalculateAverage(const Buffer& buffer, int start, int end, int width, int height) {

        for (size_t i = 0; i < buffer.size; i += 4) {
            
        }
        
        return { };
    }

    // Converts BT.601 YCbCr YUV to RGB
    RGBPix ConvertToRGB(const YUVPix& yuv) {

        // Reference: Jack, Keith. Video Demystified : A Handbook for the Digital Engineer, Elsevier Science & Technology, 2007
        float Y = yuv.lum;
        float Cb = yuv.chrom_u - 128.0f;
        float Cr = yuv.chrom_v - 128.0f;

        float R = Y + 1.402f   * Cr;
        float G = Y - 0.34414f * Cb - 0.71414f * Cr;
        float B = Y + 1.772f   * Cb;
        
        return {
            std::clamp(R, 0.0f, 255.0f),
            std::clamp(G, 0.0f, 255.0f),
            std::clamp(B, 0.0f, 255.0f)
        };
    }
    
}
