/**
 * @file Camera.h
 * @author Derek Molloy, Yassine and Hans
 * @date 20 November 2020
 * @brief Camera class header file.
 *
 * Copyright Derek Molloy, School of Electronic Engineering, Dublin City University
 * www.derekmolloy.ie
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that source code redistributions retain this notice.
 *
 * This software is provided AS IS and it comes with no warranties of any type.
 */

#ifndef PROJET_CAMERA_H
#define PROJET_CAMERA_H

#include "helpers.h"
#include "CameraSettings.h"
#include <time.h>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/**
 * Camera class wraps the opencv VideoCapture.
 */
class Camera {

public:
    /**
     * \brief Inits the default camera
     */
    Camera();
    /**
     * \brief Inits the default camera with a specific resolution
     */
    Camera(uint16_t, uint16_t);
    /**
     * \brief Destroys camera object
     */
    virtual ~Camera();
    /**
     * \brief Detects camera FPS
     */
    void detectFps();
    /**
     * \brief Records a video
     * \param path Where to save the video
     * \param videoLengthInSec self-explanatory
     */
    void recordVideo(const string& path, double videoLengthInSec);

    /**
     * Returns an empty matrix with a given size
     * \param resX self-explanatory
     * \param resY self-explanatory
     * \return Empty matrix
     */
    static Mat getBlankImageMat(const int &resX, const int &resY);

    /**
     * Returns an empty matrix same size as the current camera
     * \return Empty matrix
     */
    Mat getBlankImageMat() const;

    /**
     * Change camera's resolution
     * \param resX self-explanatory
     * \param resY self-explanatory
     */
    void changeRes(const int &resX, const int &resY);

    /**
     * Get camera settings
     * \return camera settings
     */
    const CameraSettings &getCameraSettings() const;

    /**
     * Set camera settings
     */
    void setCameraSettings(const CameraSettings &cameraSettings);

    /**
     * Get VideoCapture object
     * \return VideoCapture object
     */
    const VideoCapture &getCapture() const;

    /**
     * Set VideoCapture object
     */
    void setCapture(const VideoCapture &capture);

private:
    VideoCapture _capture;             /**< The VideoCapture object */
    CameraSettings _cameraSettings;    /**< The CameraSettings object */
};


#endif //PROJET_CAMERA_H
