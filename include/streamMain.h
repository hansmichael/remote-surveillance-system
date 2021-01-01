/**
 * @file streamMain.h
 * @author Isaac Maia, Sheriff Olaoye, Yassine and Hans
 * @date 20 November 2020
 * @brief Stream mode header file.
 *
 * OpenCV video streaming over TCP/IP
 * Server: Captures video from a webcam and send it to a client
 * by Isaac Maia
 *
 * modified by Sheriff Olaoye (sheriffolaoye.com)
 *
 */

#ifndef STREAM_MAIN_H
#define STREAM_MAIN_H

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <pthread.h>

#include <semaphore.h>
#include "../include/Camera.h"
#include "../include/constants.h"

using namespace cv;

/**
 * A structure that contains the parameters needed by the streamer
 */
struct StreamerArgs {
    int & socket;                   /**< the client socket */
    sem_t * semaphore;              /**< the main thread semaphore */
    Camera & sharedStreamingCamera; /**< the camera object */
    bool & sharedShouldStopServer;  /**< a boolean that stops the server when set to true */
};
/**
 * \brief Streaming thread
 * \param ptr Pointer to StreamerArgs object
 */
void *streamer(void *ptr);

/**
 * \brief Launches app in stream mode to stream a video
 * \param videoResX self-explanatory
 * \param videoResY self-explanatory
 * \param streamPort self-explanatory
 * \return Exit status
 */
int streamMain(uint16_t videoResX, uint16_t videoResY, u_int streamPort);

#endif //STREAM_MAIN_H