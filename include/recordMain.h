/**
 * @file recordMain.h
 * @author Yassine and Hans
 * @date 20 November 2020
 * @brief Record mode header file.
 */

#ifndef RECORD_MAIN_H
#define RECORD_MAIN_H

#include "../include/Camera.h"

using namespace std;

/**
 * \brief Launches app in record mode to record a video
 * \param videoResX self-explanatory
 * \param videoResY self-explanatory
 * \param videoLength self-explanatory
 * \return Exit status
 */
int recordMain(uint16_t videoResX, uint16_t videoResY, double videoLength);

#endif //RECORD_MAIN_H