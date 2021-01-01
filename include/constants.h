/**
 * @file constants.h
 * @author Yassine and Hans
 * @date 20 November 2020
 * @brief Defines all sorts of constants
 */

#ifndef PROJET_CONSTANTS_H
#define PROJET_CONSTANTS_H

// Message sent by client
#define SOFT_QUIT_MSG 1 // Only quits client
#define HARD_QUIT_MSG 2 // Quits server too
#define RES_PRESET_1_MSG 10
#define RES_PRESET_2_MSG 20
#define RES_PRESET_3_MSG 30
#define RES_PRESET_4_MSG 40
#define GET_FRAME_MSG 100

// Message sent by server
#define READY_MSG 101
#define IDOWN_MSG 102
#define PUSHB_MSG 103

// Messages sent by both
#define NULL_MSG 0

#define TRANSFERABLE_IMAGE_METADATA_HEADER 194

#define CLIENT_WINDOW_NAME "Projet Client"

#ifdef __APPLE__
    #define RES_PRESET_1_X 1280
    #define RES_PRESET_1_Y 720

    #define RES_PRESET_2_X 640
    #define RES_PRESET_2_Y 480

    #define RES_PRESET_3_X 640
    #define RES_PRESET_3_Y 480

    #define RES_PRESET_4_X 640
    #define RES_PRESET_4_Y 480
#else
    #define RES_PRESET_1_X 1280
    #define RES_PRESET_1_Y 720

    #define RES_PRESET_2_X 800
    #define RES_PRESET_2_Y 600

    #define RES_PRESET_3_X 320
    #define RES_PRESET_3_Y 240

    #define RES_PRESET_4_X 1280
    #define RES_PRESET_4_Y 960
#endif

/**
 * Structure that is transfered before every frame
 */
struct TransferableImageMetadata {
    uint8_t header = TRANSFERABLE_IMAGE_METADATA_HEADER; /**< Constant value to verify partial integrity of the data received */
    double width;                                        /**< Width of the image sent */
    double height;                                       /**< Height of the image sent */
};


#endif //PROJET_CONSTANTS_H
