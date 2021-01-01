/**
 * @file Camera.cpp
 * @author Derek Molloy, Yassine and Hans
 * @date 20 November 2020
 * @brief Camera class implementation.
 *
 * Copyright Derek Molloy, School of Electronic Engineering, Dublin City University
 * www.derekmolloy.ie
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that source code redistributions retain this notice.
 *
 * This software is provided AS IS and it comes with no warranties of any type.
 */

#include "../include/Camera.h"

using namespace std;
using namespace cv;
using namespace helpers;

Camera::Camera() {
    _cameraSettings.resX = getUserInputWithFallback("Resolution X", 800);
    _cameraSettings.resY = getUserInputWithFallback("Resolution Y", 600);

    _capture = VideoCapture(0);
    _capture.set(CV_CAP_PROP_FRAME_WIDTH, _cameraSettings.resX);
    _capture.set(CV_CAP_PROP_FRAME_HEIGHT, _cameraSettings.resY);

    if(!_capture.isOpened()){
        cout << "Failed to connect to the camera." << endl;
    }

    msleep(1*1000);
    detectFps();
}

Camera::Camera(uint16_t resX, uint16_t resY) {
    _cameraSettings.resX = resX;
    _cameraSettings.resY = resY;

    _capture = VideoCapture(0);
    _capture.set(CV_CAP_PROP_FRAME_WIDTH, _cameraSettings.resX);
    _capture.set(CV_CAP_PROP_FRAME_HEIGHT, _cameraSettings.resY);

    if(!_capture.isOpened()){
        cout << "Failed to connect to the camera." << endl;
    }
}

void Camera::detectFps() {
        Mat frame;

        struct timespec start, end;

        msleep(1*1000);
        int frames=2;
        for(int i=0; i<frames; i++){
            _capture >> frame;
            if(frame.empty()){
                cout << "Failed to capture an image" << endl;
                return;
            }
        }

        clock_gettime( CLOCK_REALTIME, &start );
        frames=10;
        for(int i=0; i<frames; i++){
            _capture >> frame;
            if(frame.empty()){
                cout << "Failed to capture an image" << endl;
                return;
            }
        }
        clock_gettime( CLOCK_REALTIME, &end );
        double difference = double(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec)/(double)1000000000.0;

        cout << "It took " << difference << " seconds to process " << frames << " frames" << endl;
        cout << "Capturing and processing " << frames/difference << " frames per second " << endl;

        _cameraSettings.fps = frames/difference;
}

const CameraSettings &Camera::getCameraSettings() const {
    return _cameraSettings;
}

void Camera::setCameraSettings(const CameraSettings &cameraSettings) {
    _cameraSettings = cameraSettings;
}

const VideoCapture &Camera::getCapture() const {
    return _capture;
}

void Camera::setCapture(const VideoCapture &capture) {
    _capture = capture;
}

void Camera::recordVideo(const string &path, double videoLengthInSec) {
    Mat frame;
    detectFps();
    VideoWriter video(path, CV_FOURCC('M','J','P','G'), _cameraSettings.fps, Size(_cameraSettings.resX,_cameraSettings.resY), true);

    int frames = int(_cameraSettings.fps * videoLengthInSec);
    for(int i=0; i<frames; i++){
        _capture >> frame;
        if(frame.empty()){
            cout << "Failed to capture a frame" << endl;
            return;
        }
        video.write(frame);
        if (i % int(_cameraSettings.fps) == 0) { cout << "Seconde : " << i / int(_cameraSettings.fps) << endl; }
    }
    video.release();
}

Camera::~Camera() {
    _capture.release();
    if(!_capture.isOpened()){
        cout << "Camera now closed" << endl;
    }
}

Mat Camera::getBlankImageMat() const {
    return Camera::getBlankImageMat(_cameraSettings.resX, _cameraSettings.resY);
}

Mat Camera::getBlankImageMat(const int &resX, const int &resY) {
    Mat image = Mat::zeros(resY, resX, CV_8UC3);

    if(!image.isContinuous()){
        image = image.clone();
    }

    return image;
}

void Camera::changeRes(const int &resX, const int &resY) {
    _cameraSettings.resX = resX;
    _cameraSettings.resY = resY;

    _capture.set(CV_CAP_PROP_FRAME_WIDTH, _cameraSettings.resX);
    _capture.set(CV_CAP_PROP_FRAME_HEIGHT, _cameraSettings.resY);

    msleep(1*500);
}
