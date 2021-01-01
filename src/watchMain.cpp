/**
 * @file watchMain.cpp
 * @author Steve Tuenkam, Sheriff Olaoye, Yassine and Hans
 * @date 20 November 2020
 * @brief Stream mode implementation file.
 *
 * OpenCV video streaming over TCP/IP
 * Client: Receives video from server and display it
 * by Steve Tuenkam
 *
 * modified by Sheriff Olaoye (sheriffolaoye.com)
 *
 */

#ifdef __x86_64__
#include "watchMain.h"

using namespace cv;
using namespace std;
using namespace helpers;
using namespace zbar;

int watchMain (const char *serverIP, u_int serverPort) {
    uint32_t message = NULL_MSG;

    bool shouldStopReceivingFrames = false;
    while (!shouldStopReceivingFrames) {
        // Initialize TCP-IP client
        int socket_fd;

        struct sockaddr_in serverAddr;
        socklen_t addrLen = sizeof(struct sockaddr_in);

        if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
            cerr << "socket() failed" << endl;
        }

        serverAddr.sin_family = PF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(serverIP);
        serverAddr.sin_port = htons(serverPort);

        while (connect(socket_fd, (sockaddr*)&serverAddr, addrLen) < 0) {
            cerr << "connect() failed!" << endl;
            msleep(500);
        }

        ImageScanner scanner;
        scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

        // OpenCV Code
        Mat image = Camera::getBlankImageMat(0, 0);
        int imageSize = image.total() * image.elemSize();

        TransferableImageMetadata data;
        int dataSize = sizeof(struct TransferableImageMetadata);

        int bytes = 0;
        int lastPressedKey = -1;
        int recvErrorsCount = 0;

        namedWindow(CLIENT_WINDOW_NAME, 1);
        cvui::init(CLIENT_WINDOW_NAME);
        bool isMenuHidden = false;
        bool isEdgesMode = false;
        double edgesTreshhold1 = 0;
        double edgesTreshhold2 = 30;
        int edgesApertureSize = 3;

        uint32_t lastServerStatus = NULL_MSG;

        while (!shouldStopReceivingFrames) {
            bool shouldScreenshot = false;
            // Update last pressed key
            lastPressedKey = waitKey(6);

            // Verify last pressed key and set message
            switch (lastPressedKey) {
                case 27 : // ESC
                    cout << "ESC was pressed." << endl;
                    message = HARD_QUIT_MSG;
                    shouldStopReceivingFrames = true;
                    break;
                case 'q':
                    cout << "q was pressed." << endl;
                    message = SOFT_QUIT_MSG;
                    shouldStopReceivingFrames = true;
                    break;
                case '1':
                    cout << "1 was pressed." << endl;
                    message = RES_PRESET_1_MSG;
                    break;
                case '2':
                    cout << "2 was pressed." << endl;
                    message = RES_PRESET_2_MSG;
                    break;
                case '3':
                    cout << "3 was pressed." << endl;
                    message = RES_PRESET_3_MSG;
                    break;
                case '4':
                    cout << "4 was pressed." << endl;
                    message = RES_PRESET_4_MSG;
                    break;
                case 'm':
                    isMenuHidden = !isMenuHidden;
                default :
                    message = GET_FRAME_MSG;
                    break;
            }

            // Receive server status
            if ((bytes = recv(socket_fd, &lastServerStatus, sizeof(lastServerStatus), 0)) < 0) {
                cerr << "recv failed, received bytes = " << bytes << endl;
                recvErrorsCount += 1;

                if (recvErrorsCount > 5) {
                    cerr << "Too many errors receiving from client... Quitting... Retrying in 1 sec..." << endl;

                    message = SOFT_QUIT_MSG;
                    if (send(socket_fd, &message, sizeof(message), 0) == -1) {
                        cerr << "Send failed" << endl;
                    }

                    close(socket_fd);
                    msleep(1*1000);
                    break;
                }
            }

            switch (lastServerStatus) {
                case READY_MSG:
                    break;
                case IDOWN_MSG:
                    if (message == GET_FRAME_MSG) message = NULL_MSG;
                    break;
                case PUSHB_MSG:
                    // Fork and screenshot
                    shouldScreenshot = true;
                    break;
                default:
                    break;
            }

            // Send message
            if (send(socket_fd, &message, sizeof(message), 0) == -1) {
                cerr << "Send failed" << endl;
            }

            if (message != GET_FRAME_MSG) {
                message = NULL_MSG;
                continue;
            }

            // Receive metadata
            if ((bytes = recv(socket_fd, &data, dataSize, MSG_WAITALL)) == -1) {
                cerr << "recv failed, received bytes = " << bytes << endl;
                recvErrorsCount += 1;

                if (recvErrorsCount > 5) {
                    cerr << "Too many errors receiving from client... Quitting... Retrying in 1 sec..." << endl;

                    message = SOFT_QUIT_MSG;
                    if (send(socket_fd, &message, sizeof(message), 0) == -1) {
                        cerr << "Send failed" << endl;
                    }

                    close(socket_fd);
                    msleep(1*1000);
                    break;
                }
            }

            // Validate data received
            if (data.header != TRANSFERABLE_IMAGE_METADATA_HEADER) {
                cerr << "Fell out of sync, restarting." << endl;

                message = SOFT_QUIT_MSG;
                if (send(socket_fd, &message, sizeof(message), 0) == -1) {
                    cerr << "Send failed" << endl;
                }

                close(socket_fd);
                msleep(1*1000);
                break;
            }

            // Fix image size if wrong
            if (image.size().height != data.height || image.size().width != data.width) {
                image.release();
                image = Camera::getBlankImageMat(data.width, data.height);
                imageSize = image.total() * image.elemSize();

                // Debug
                cout <<  "HEADER " << (int)data.header <<
                " HEIGHT " << data.width <<
                ", WIDTH " << data.height << endl;
            }

            // Receive latest frame
            if ((bytes = recv(socket_fd, image.data, imageSize, MSG_WAITALL)) == -1) {
                cerr << "recv failed, received bytes = " << bytes << endl;
                recvErrorsCount += 1;

                if (recvErrorsCount > 5) {
                    cerr << "Too many errors receiving from client... Quitting... Retrying in 1 sec..." << endl;
                    recvErrorsCount += 1;

                    message = SOFT_QUIT_MSG;
                    if (send(socket_fd, &message, sizeof(message), 0) == -1) {
                        cerr << "Send failed" << endl;
                    }

                    close(socket_fd);
                    msleep(1*1000);
                    break;
                }
            }

            Mat finalImage;

            if (isEdgesMode) {
                edgesApertureSize = 2 * ( (int)(edgesApertureSize / 2.0f) ) + 1;

                cvtColor(image, finalImage, CV_BGR2GRAY);
                Canny(finalImage, finalImage, edgesTreshhold1, edgesTreshhold2, edgesApertureSize);
            } else {
                finalImage = image;
            }

            // Overlay image
            if (!isMenuHidden) {
                int menuX = data.width * 0.02;
                int menuY = data.height * 0.02;
                int menuWidth = data.width * 0.2;
                int menuHeight = data.height * 0.75;

                int mainColumnX = menuX + menuWidth * 0.05;
                int mainColumnY = menuY + menuHeight * 0.075;
                int mainColumnWidth = menuWidth * 0.8;
                int mainColumnHeight = menuHeight * 0.8;

                double baseFontScale = 0.4 / (16./9.) * (data.width/data.height);
                double titleFontScale = 1. * baseFontScale;
                double normalFontScale = .9 * baseFontScale;

                cvui::window(finalImage, menuX, menuY, menuWidth, menuHeight, "Menu");
                cvui::beginColumn(finalImage, mainColumnX, mainColumnY, mainColumnWidth, mainColumnHeight, mainColumnHeight * 0.02);
                    cvui::text("Hotkeys :", titleFontScale);
                    cvui::text("---------------", normalFontScale);
                    cvui::text("'m' Hide menu", normalFontScale);
                    cvui::text("'1' " + std::to_string(RES_PRESET_1_Y) + "x" + std::to_string(RES_PRESET_1_X), normalFontScale);
                    cvui::text("'2' " + std::to_string(RES_PRESET_2_Y) + "x" + std::to_string(RES_PRESET_2_X), normalFontScale);
                    cvui::text("'3' " + std::to_string(RES_PRESET_3_Y) + "x" + std::to_string(RES_PRESET_3_X), normalFontScale);
                    cvui::text("'4' " + std::to_string(RES_PRESET_4_Y) + "x" + std::to_string(RES_PRESET_4_X), normalFontScale);
                    cvui::text("'q' Quit client", normalFontScale);
                    cvui::text("'ESC' Quit client and server", normalFontScale);

                    cvui::space(2);

                    cvui::text("Misc. :", titleFontScale);
                    cvui::text("---------------", normalFontScale);
                    cvui::checkbox("Edges mode", &isEdgesMode);
                    if (isEdgesMode) {
                        cvui::trackbar(menuWidth * 0.8, &edgesTreshhold1, (double)0, (double)300);
                        cvui::trackbar(menuWidth * 0.8, &edgesTreshhold2, (double)0, (double)300);
                        cvui::trackbar(menuWidth * 0.8, &edgesApertureSize, (int)3, (int)7);
                    }

                cvui::endColumn();
            }

#ifdef __APPLE__
            string decodedString = helpers::detectAndDecodeQR(finalImage);
            if (!decodedString.empty()) {
                cout << "Decoded string : " << decodedString << endl;
                if (!isMenuHidden) {
                    double baseFontScale = 0.4 / (16./9.) * (data.width/data.height);
                    cvui::text(finalImage, data.width / 3, data.height / 2,  decodedString, baseFontScale);
                }
            }
#endif

            if (shouldScreenshot) {
                bool isParentProcess = fork();
                if (!isParentProcess) {
                    string decodedString = helpers::detectAndDecodeQR(finalImage);
                    imwrite("Picture_" + GetCurrentTimeForFileName() + "_.png", finalImage);
                    cout << "Decoded string : " << decodedString << endl;

                    if (!isMenuHidden) {
                        double baseFontScale = 0.4 / (16./9.) * (data.width/data.height);
                        cvui::text(finalImage, data.width / 3, data.height / 2,  decodedString, baseFontScale);
                        cvui::update();
                    }

                    // Connect to port 4100
                    int socket_fd;

                    struct sockaddr_in serverAddr;
                    socklen_t addrLen = sizeof(struct sockaddr_in);

                    if ((socket_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
                        cerr << "socket() failed" << endl;
                    }

                    serverAddr.sin_family = PF_INET;
                    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
                    serverAddr.sin_port = htons(4100);

                    int amount_of_retries = 0;
                    while (connect(socket_fd, (sockaddr*)&serverAddr, addrLen) < 0) {
                        cerr << "connect() failed!" << endl;
                        msleep(500);
                        if (amount_of_retries++ > 5) {
                            close(socket_fd);
                            exit(1);
                        }
                    }

                    // Send decoded string
                    uint32_t dataLength = decodedString.size();
                    send(socket_fd, &dataLength, sizeof(uint32_t), 0);
                    send(socket_fd, decodedString.c_str(), decodedString.size(), 0);

                    close(socket_fd);
                    exit(0);
                }
            }

            if (!isMenuHidden) {
                // Update cvui internal stuff
                cvui::update();
            }

            // Display last frame received
            imshow(CLIENT_WINDOW_NAME, finalImage);

            // Reset message
            message = NULL_MSG;
        }

        // Close socket
        close(socket_fd);
    }

    cout << "Exiting with code 0" << endl;

    return 0;
}
#endif //__x86_64__