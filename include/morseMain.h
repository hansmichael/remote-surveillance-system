/**
 * @file morseMain.h
 * @author Yassine and Hans
 * @date 27 November 2020
 * @brief morse mode header file.
 */

#ifndef MORSE_MAIN_H
#define MORSE_MAIN_H

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include "PWMSongParser.h"

using namespace std;

/**
 * \brief Launches app in morse mode to play morse
 * \param morsePort self-explanatory
 * \return Exit status
 */
int morseMain(u_int morsePort);

#endif