/*
Parts of the program are based on Marcus Folkesson's V4L2 tutorial:
https://www.marcusfolkesson.se/blog/capture-a-picture-with-v4l2/
*/

#include "recorder.h"
#include "processor.h"

#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <poll.h>

#include <linux/videodev2.h>

int Recorder::requestBuffer() {
    struct v4l2_requestbuffers request = {};

    request.count = bfrCnt;
    request.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    request.memory = V4L2_MEMORY_MMAP;

    int res = ioctl(fd, VIDIOC_REQBUFS, &request);
    if (res == -1) {
        perror("Buffer request failed");
        exit(res);
    }
    return request.count;
}

int Recorder::queryBuffer(int ind, Buffer& buffer) {
    struct v4l2_buffer buf = {};

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = ind;

    int res = ioctl(fd, VIDIOC_QUERYBUF, &buf);
    if (res == -1) {
        perror("Failed to query buffer");
        exit(res);
    }

    buffer.data = static_cast<unsigned char*>(mmap(
        nullptr,
        buf.length,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        buf.m.offset
    ));

    return buf.length;
    
}

int Recorder::queueBuffer(int ind) {
    struct v4l2_buffer buf = {};

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = ind;

    int res = ioctl(fd, VIDIOC_QBUF, &buf);
    if (res == -1) {
        perror("Failed to query buffer");
        exit(res);
    }

    return buf.bytesused;
    
}

int Recorder::setFormat() {
    struct v4l2_format format = {};

    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = width;
    format.fmt.pix.height = height;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    format.fmt.pix.field = V4L2_FIELD_NONE;

    int res = ioctl(fd, VIDIOC_S_FMT, &format);
    if (res == -1) {
        perror("Format failed");
        exit(res);
    }
    return res;
    
}

int Recorder::dequeueBuffer() {
    struct v4l2_buffer buffer = {};

    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory = V4L2_MEMORY_MMAP;
    buffer.index = 0;
    
    int res = ioctl(fd, VIDIOC_DQBUF, &buffer);
    if (res == -1) {
        perror("Format failed");
        exit(res);
    }
    return buffer.index;
    
}

int Recorder::startStream() {
    auto type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int res = ioctl(fd, VIDIOC_STREAMON, &type);
    if (res == -1) {
        perror("VIDIOC_STREAMON");
    }

    running = true;

    return res >= 0;
}

int Recorder::stopStream() {
    auto type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int res = ioctl(fd, VIDIOC_STREAMOFF, &type);
    if (res == -1) {
        perror("VIDIOC_STREAMOFF");
    }

    running = false;

    return res >= 0;
}

Recorder::Recorder(const std::string& device, int width, int height)
    : device(device),
    width(width),
    height(height)
{
    fd = open(device.data(), O_RDWR);

    if (fd < 0) {
        perror(device.data());
        exit(fd);
    }
}

int Recorder::unInit() {
    for (auto& buf : buffers) {
        munmap(buf.data, buf.size);
    }

    close(fd);
    return 0;
}

Recorder::~Recorder() {
    if (running) stopStream();
    unInit();
}

int Recorder::init() {
    setFormat();

    auto nbufs = requestBuffer();
    if (nbufs > bfrCnt) {
        std::cout << "Buffer count too low, set at least: " << nbufs << std::endl;
        return -1;
    }

    buffers.resize(bfrCnt);

    for (int i = 0; i < bfrCnt; ++i) {
        bufSize = queryBuffer(i, buffers[i]);
        buffers[i].size = bufSize;
        queueBuffer(i);
    }
    return 0;
}

void Recorder::saveToFile(const std::string& filename, int ind) {
    int file = open(filename.data(), O_RDWR | O_CREAT, 0666);
    write(file, buffers[ind].data, buffers[ind].size);
    close(file);
}

int Recorder::run() {

    std::cout << "intitialising" << std::endl;

    if (init() == -1) {
        return -1;
    }

    startStream();

    // Just for the duration of development
    const int pictureCount = 10;
    
    for (int i = 0; i < pictureCount; ++i) {
        struct pollfd pfd = { fd, POLLIN, 0 };
        int r = poll(&pfd, 1, 2000);

        if (r == -1) {
            throw std::system_error(errno, std::generic_category(), "Frame wait failed");
        } else if (r == 0) {
            std::cerr << "Missed a frame" << std::endl;
            continue;
        }

        int ind = dequeueBuffer();
        // Write to file now to verify the correctness
        saveToFile("captures/out"+std::to_string(i)+".yuv", ind);
        auto pix = Processor::CalculateAverage(buffers[ind], width, height);
        std::cout << "Lum: " << unsigned(pix.lum) << " U and V: " << unsigned(pix.chrom_u) << " : " << unsigned(pix.chrom_v) << std::endl;
    }

    stopStream();
    
    std::cout << "Ran succesfully" << std::endl;
    
    return 0;
}
