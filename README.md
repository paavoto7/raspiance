# Raspiance
Raspiance is a RaspberryPi TV ambiance light controller. It uses webcam feed to count average color on screen segments and set the respective LED's to these averages.

The project is currently in development and not yet production ready.

## Getting started

### Requirements
- C++17
- CMake
- videodev2 header file

Requires a webcam that should be YUYV format and YCbCr BT.601 compatible. The default path for the video feed is */dev/video2/*.

The project is aimed at RaspberryPi and therefore uses POSIX system calls.

### Building
```
git clone https://github.com/paavoto7/raspiance.git
cd raspiance
mkdir build && cd build
cmake ..
```
Use Make for building
```
make
./src/raspiance
```
or CMake
```
cmake --build .
./src/raspiance
```

You can also use the following to enable O3 and march=native
```
cmake .. -DUSE_PERFORMANCE_FLAGS=ON
```

## Feature status

### Implemented
- Reads webcam feed via V4L2.
- Can calculate YUV average for whole image and also for specified segments.
- Is able to convert YUV average into RGB.

### Planned
- Network communication to Hue Bridge
    - Either implemented directly in C++ or with Python
    - Needs to use Hue Entertainment API, which requires more than simple HTTP
- Calibration mode
    - Used for aiming the camera properly towards the TV

### Miscellaneous
The src directory contains the actual c++ program for now.\
The py directory contains the old OpenCV python scipt.

Convert.sh script was used for converting the raw YUYV format images to PNG's.
