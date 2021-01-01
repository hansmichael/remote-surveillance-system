/**
 * @file watchMain.h
 * @author Steve Tuenkam, Sheriff Olaoye, Yassine and Hans
 * @date 20 November 2020
 * @brief Stream mode header file.
 *
 * OpenCV video streaming over TCP/IP
 * Client: Receives video from server and display it
 * by Steve Tuenkam
 *
 * modified by Sheriff Olaoye (sheriffolaoye.com)
 *
 */

#ifdef __x86_64__
#ifndef WATCH_MAIN_H
#define WATCH_MAIN_H

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../include/constants.h"
#include "../include/Camera.h"
#include "helpers.h"
#include "cvui.h"

using namespace std;
/**
 * \brief Launches app in watch mode to watch a stream
 * \param serverIP self-explanatory
 * \param serverPort self-explanatory
 * \return Exit status
 */
int watchMain (const char *serverIP, u_int serverPort);

#endif //WATCH_MAIN_H
#endif //__x86_64__