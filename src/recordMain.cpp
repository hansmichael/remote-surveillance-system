/**
 * @file recordMain.cpp
 * @author Yassine and Hans
 * @date 20 November 2020
 * @brief Record mode implementation.
 */

#include "../include/recordMain.h"

using namespace std;

int recordMain(uint16_t videoResX, uint16_t videoResY, double videoLength) {
    Camera camera(videoResX, videoResY);
    camera.recordVideo("capture-liv1.avi", videoLength);
    return 0;
}