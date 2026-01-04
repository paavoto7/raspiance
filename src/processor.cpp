#include "processor.h"

#include <algorithm>
#include <stdexcept>

// 	Pixel Format : 'YUYV' (YUYV 4:2:2)
// YCbCr : BT.601

namespace Processor {

    // Calculate the average YUV values for the whole buffer
    void CalculateAverage(const Buffer& buffer, YUVPix& yuvpix, int width, int height) {
        uint64_t lum = 0;
        uint64_t chrom_u = 0;
        uint64_t chrom_v = 0;
        
        const auto size = buffer.size;
        for (std::size_t i = 0; i < size; i += 4) {
            
            lum += buffer.data[i] + buffer.data[i + 2];
            chrom_u += buffer.data[i + 1];
            chrom_v += buffer.data[i + 3];
        }

        const auto pixCount = width * height;

        yuvpix.lum = static_cast<uint8_t>(lum / pixCount);
        yuvpix.chrom_u = static_cast<uint8_t>(chrom_u / (pixCount / 2));
        yuvpix.chrom_v = static_cast<uint8_t>(chrom_v / (pixCount / 2));
    }

    // Calculate the average YUV values for a number of segments
    void CalculateAverage(const Buffer& buffer, std::vector<Segment>& segments, int width, int height) {

        for (auto& seg: segments) {

            // Throw error later
            if (!seg.isInBounds(width * 2, height)) {
                continue;
            }

            if (seg.xi % 4 != 0) {
                throw std::runtime_error("Segment has non-four divisible starting value.");
            }

            uint64_t lum = 0;
            uint64_t chrom_u = 0;
            uint64_t chrom_v = 0;
            
            // Loop over rows
            for (int i = seg.yi; i < seg.yj; ++i) {
                auto row = i * (width * 2);

                // Loop over columns
                for (int j = seg.xi; j < seg.xj; j += 4) { 
                    auto pos = row + j;
                    
                    lum += buffer.data[pos] + buffer.data[pos + 2];
                    chrom_u += buffer.data[pos + 1];
                    chrom_v += buffer.data[pos + 3];
                }
            }
            const auto pixCount = (seg.xj - seg.xi) / 2 * (seg.yj - seg.yi);
            // YUVPix pix = ;
            seg.avg = {
                static_cast<uint8_t>(lum / pixCount),
                static_cast<uint8_t>(chrom_u / (pixCount / 2)),
                static_cast<uint8_t>(chrom_v / (pixCount / 2))
            };
        }
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
