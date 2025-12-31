#ifndef RECORDER_H
#define RECORDER_H

#include <string>
#include <vector>

#include "buffer.h"

class Recorder {
public:

    Recorder(
        const std::string& device = "/dev/video2", // For ease of testing
        int width = 640,
        int height = 360
    );

    ~Recorder();

    int run();
    
private:
    int fd;
    constexpr static int bfrCnt = 20;
    std::vector<Buffer> buffers;
    int bufSize = 0;

    bool running = false;

    const std::string device;
    
    const int width;
    const int height;

    int stopStream();
    int startStream();
    
    int dequeueBuffer();
    
    int queueBuffer(int ind);
    int queryBuffer(int ind, Buffer& buffer);
    int requestBuffer();

    int setFormat();
    int init();
    int unInit();

    void saveToFile(const std::string& filename, int ind);

};

#endif // !RECORDER_H
