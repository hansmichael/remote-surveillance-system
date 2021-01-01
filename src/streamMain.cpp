/**
 * @file streamMain.cpp
 * @author Isaac Maia, Sheriff Olaoye, Yassine and Hans
 * @date 20 November 2020
 * @brief Stream mode implementation file.
 *
 * OpenCV video streaming over TCP/IP
 * Server: Captures video from a webcam and send it to a client
 * by Isaac Maia
 *
 * modified by Sheriff Olaoye (sheriffolaoye.com)
 *
 */

#include "../include/streamMain.h"

using namespace cv;

int streamMain(uint16_t videoResX, uint16_t videoResY, u_int streamPort) {
    // Initialize semaphore;
    sem_t *semaphore = sem_open("streamSem", 0, 1);

    // Initialize TCP-IP server
    int localSocket, remoteSocket;
    struct sockaddr_in localAddr, remoteAddr;
    int addrLen = sizeof(struct sockaddr_in);

    pthread_t stream_thread;

    localSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (localSocket == -1){
        perror("socket() call failed!!");
    }

    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons(streamPort);

    int result = ::bind(localSocket, (struct sockaddr *)&localAddr, sizeof(localAddr));
    if(result < 0) {
        perror("Can't bind() socket");
        exit(1);
    }

    // Listening
    listen(localSocket , 3);
    cout <<  "Server listening on port: " << streamPort << endl;

    // Initialize camera
    Camera sharedStreamingCamera(videoResX, videoResY);

    // Enable GPIO 228
    helpers::enableGPIO(228, "in");

    // Accept connection from an incoming client
    bool sharedShouldStopServer = false;
    bool safeShouldStopServer = sharedShouldStopServer;
    while(!safeShouldStopServer){
        cout << "Waiting for new connection" << endl;
        remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);

        if (remoteSocket < 0) {
            perror("accept failed!");
            exit(1);
        }

        cout << "Connection accepted" << endl;

        // Start streamer
        StreamerArgs streamerArgs{
            remoteSocket,
            semaphore,
            sharedStreamingCamera,
            sharedShouldStopServer
        };
        pthread_create(&stream_thread, NULL, streamer, &streamerArgs); cout << "Streamer created" << endl;

        // Limit to 1 remote connection at a time
        pthread_join(stream_thread, NULL);

        // Make sure to close the remote socket
        close(remoteSocket);

        // Quit if we received a hard quit
        sem_wait(semaphore);
        if (sharedShouldStopServer) safeShouldStopServer = true;
        sem_post(semaphore);
    }

    sem_unlink ("streamSem");
    cout << "Quitting server" << endl;
    return 0;
}

void *streamer(void *ptr){
    cout << "Streamer running" << endl;

    StreamerArgs streamerArgs = *(StreamerArgs *)ptr;
    uint32_t lastReceivedMessage = NULL_MSG;
    uint32_t messageToSend = NULL_MSG;

    bool isThereLight = true;
    bool isButtonPushed = false;

    VideoCapture capture = streamerArgs.sharedStreamingCamera.getCapture();

    Mat image = streamerArgs.sharedStreamingCamera.getBlankImageMat();
    int imageSize = image.total() * image.elemSize();

    TransferableImageMetadata data;
    int dataSize = sizeof(struct TransferableImageMetadata);

    int sentBytes = 0;
    int receivedBytes = 0;
    int recvErrorsCount = 0;

    bool shouldStopStreamer = false;
    while(!shouldStopStreamer){
#ifndef __x86_64__
        // Lecture Capteur
        isThereLight = helpers::isThereLight();
        // Lecture Boutton
        isButtonPushed = helpers::readGPIO(228, true);
#endif
        // Set message to send
        if (!isThereLight) messageToSend = IDOWN_MSG;
        else if (isButtonPushed) messageToSend = PUSHB_MSG;
        else messageToSend = READY_MSG;

        if (isButtonPushed) {
            bool isParentProcess = fork();
            if (!isParentProcess) {
                char *argv[6] = {strdup("./Projet"), strdup("-m"), strdup("morse"), strdup("-p"), strdup("4100"), NULL};;
                execvp("./Projet", argv);
                exit(0);
            }
        }

        if ((sentBytes = send(streamerArgs.socket, &messageToSend, sizeof(messageToSend), 0) < 0)) {
            cerr << "bytes = " << sentBytes << endl;
            break;
        }

        // Receive message
        receivedBytes = recv(streamerArgs.socket, &lastReceivedMessage, sizeof(lastReceivedMessage), 0);

        if (receivedBytes < 0){
            recvErrorsCount += 1;
            if (recvErrorsCount > 5) {
                cerr << "Too many errors receiving from client... Quitting.." << endl;
                break;
            }

            continue;
        }

        // Take action in this thread
        switch (lastReceivedMessage) {
            case SOFT_QUIT_MSG:
                shouldStopStreamer = true;
                continue;
            case HARD_QUIT_MSG:
                shouldStopStreamer = true;
                sem_wait(streamerArgs.semaphore);
                streamerArgs.sharedShouldStopServer = true;
                sem_post(streamerArgs.semaphore);
                continue;
            case RES_PRESET_1_MSG:
                streamerArgs.sharedStreamingCamera.changeRes(RES_PRESET_1_X, RES_PRESET_1_Y);
                goto endOfStreamLoop;
            case RES_PRESET_2_MSG:
                streamerArgs.sharedStreamingCamera.changeRes(RES_PRESET_2_X, RES_PRESET_2_Y);
                goto endOfStreamLoop;
            case RES_PRESET_3_MSG:
                streamerArgs.sharedStreamingCamera.changeRes(RES_PRESET_3_X, RES_PRESET_3_Y);
                goto endOfStreamLoop;
            case RES_PRESET_4_MSG:
                streamerArgs.sharedStreamingCamera.changeRes(RES_PRESET_4_X, RES_PRESET_4_Y);
                goto endOfStreamLoop;
            case NULL_MSG:
                goto endOfStreamLoop;
            case GET_FRAME_MSG:
            default:
                break;
        }

        if (!isThereLight) goto endOfStreamLoop;

        data.width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
        data.height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

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

        // Read frame
        capture >> image;

        // Send Metadata
        if ((sentBytes = send(streamerArgs.socket, &data, dataSize, 0)) < 0){
            cerr << "bytes = " << sentBytes << endl;
            break;
        }

        // Send frame
        if ((sentBytes = send(streamerArgs.socket, image.data, imageSize, 0)) < 0){
            cerr << "bytes = " << sentBytes << endl;
            break;
        }

    endOfStreamLoop:
        lastReceivedMessage = NULL_MSG;
    }

    cout << "Streamer thread quitting..." << endl;
    close(streamerArgs.socket);

    return 0;
}