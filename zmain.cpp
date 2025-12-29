#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/videodev2.h>


int requestBuffer(int fd, int count) {
    struct v4l2_requestbuffers request = {};

    request.count = count;
    request.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    request.memory = V4L2_MEMORY_MMAP;

    int res = ioctl(fd, VIDIOC_REQBUFS, &request);
    if (res == -1) {
        perror("Buffer request failed");
        exit(res);
    }
    return request.count;
}

int queryBuffer(int fd, int ind, unsigned char** buffer) {
    struct v4l2_buffer buf = {};

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = ind;

    int res = ioctl(fd, VIDIOC_QUERYBUF, &buf);
    if (res == -1) {
        perror("Failed to query buffer");
        exit(res);
    }

    *buffer = static_cast<unsigned char*>(mmap(
        NULL,
        buf.length,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        buf.m.offset
    ));

    return buf.length;
    
}

int queueBuffer(int fd, int ind) {
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

int setFormat(int fd) {
    struct v4l2_format format = {};

    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = 640;
    format.fmt.pix.height = 360;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    format.fmt.pix.field = V4L2_FIELD_NONE;

    int res = ioctl(fd, VIDIOC_S_FMT, &format);
    if (res == -1) {
        perror("Format failed");
        exit(res);
    }
    return res;
    
}

int dequeueBuffer(int fd) {
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

int startStream(int fd) {
    auto type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int res = ioctl(fd, VIDIOC_STREAMON, &type);
    if (res == -1) {
        perror("VIDIOC_STREAMON");
    }

    return res >= 0;
}

int stopStream(int fd) {
    auto type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int res = ioctl(fd, VIDIOC_STREAMOFF, &type);
    if (res == -1) {
        perror("VIDIOC_STREAMOFF");
    }

    return res >= 0;
}

int main() {
    constexpr int bfrCnt = 20;
    unsigned char* buffers[bfrCnt];

    const char* device = "/dev/video2";
    auto fd = open(device, O_RDWR);

    if (fd < 0) {
        perror(device);
        return fd;
    }

    setFormat(fd);

    auto nbufs = requestBuffer(fd, bfrCnt);
    if (nbufs > bfrCnt) {
        std::cout << "Buffer count too low, set at least: " << nbufs << std::endl;
        return -1;
    }

    int size = 0;
    for (int i = 0; i < bfrCnt; ++i) {
        size = queryBuffer(fd, i, &buffers[i]);

        queueBuffer(fd, i);
    }

    startStream(fd);

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    struct timeval tv = {};
    tv.tv_sec = 2;

    int r = select(fd + 1, &fds, NULL, NULL, &tv);
    if (r == -1) {
        perror("Frame wait fialed");
        return 1;
    }

    int ind = dequeueBuffer(fd);

    int file = open("out.yuv", O_RDWR | O_CREAT, 0666);
    write(file, buffers[ind], size);

    stopStream(fd);

    for (int i = 0; i < bfrCnt; ++i) {
        munmap(buffers[i], size);
    }
    close(file);
    
    std::cout << "Opened: " << fd << std::endl;

    close(fd);
    
    return 0;
}
