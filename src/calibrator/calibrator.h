#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <glad/gl.h>
#include <GLFW/glfw3.h>

// Will be responsible for "calibrating" the webcam
// That should mean aligning the camera correctly
// and perhaps adjusting width and height.
class Calibrator {
public:
    Calibrator(/* args */);
    ~Calibrator();

private:
    /* data */
};


#endif // !CALIBRATOR_H
