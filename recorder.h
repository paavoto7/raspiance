#ifndef RECORDER_H
#define RECORDER_H

#include <string>

class Recorder {
public:

    Recorder(
        const std::string& device = "/dev/video2",
        int width = 640,
        int height = 360
    );

    ~Recorder();

    int run();
    
private:
    int fd;
    constexpr static int bfrCnt = 20;
    unsigned char* buffers[bfrCnt];
    int bufSize = 0;

    const int width;
    const int height;

    const std::string device;

    int stopStream();
    int startStream();
    
    int dequeueBuffer();
    
    int queueBuffer(int ind);
    int queryBuffer(int ind, unsigned char** buffer);
    int requestBuffer();

    int setFormat();
    int init();

    void saveToFile(const std::string& filename, int ind);

};

#endif // !RECORDER_H
