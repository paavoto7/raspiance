#ifndef BUFFER_H
#define BUFFER_H

#include <cstddef>

// A helper struct to manage the data
struct Buffer {
    unsigned char* data = nullptr;
    std::size_t size = 0;
};

#endif // !BUFFER_H
