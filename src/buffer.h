#ifndef BUFFER_H
#define BUFFER_H

// A helper struct to manage the data
struct Buffer {
    unsigned char* data = nullptr;
    int size = 0;
};

#endif // !BUFFER_H
