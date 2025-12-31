#include "recorder.h"

int main() {
    Recorder recorder;

    // No real error handling for now
    try {
        recorder.run();
        return 0;
    } catch (const std::exception& e) {
        return 1;
    }
     
}
