/**
 * @file morseMain.cpp
 * @author Yassine and Hans
 * @date 27 November 2020
 * @brief Morse mode implementation.
 */


#include <vector>
#include "../include/morseMain.h"

int morseMain(u_int morsePort) {
    // Initialize TCP-IP server
    int localSocket, remoteSocket;
    struct sockaddr_in localAddr, remoteAddr;
    int addrLen = sizeof(struct sockaddr_in);

    localSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (localSocket == -1){
        perror("socket() call failed!!");
    }

    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons(morsePort);

    int result = ::bind(localSocket, (struct sockaddr *)&localAddr, sizeof(localAddr));
    if(result < 0) {
        perror("Can't bind() socket");
        exit(1);
    }

    // Listening
    listen(localSocket , 3);
    cout <<  "Server listening on port: " << morsePort << endl;


    // Accept connection from an incoming client
    cout << "Waiting for new connection" << endl;
    remoteSocket = accept(localSocket, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrLen);

    if (remoteSocket < 0) {
        perror("accept failed!");
        exit(1);
    }

    cout << "Connection accepted" << endl;

    // Wait for a message to play
    try {
        uint32_t dataLength;
        recv(remoteSocket, &dataLength, sizeof(uint32_t), 0);
//        dataLength = dataLength - 1;

        std::vector<uint8_t> rcvBuf;
        rcvBuf.resize(dataLength,0x00);

        recv(remoteSocket, &(rcvBuf[0]), dataLength, 0);

        std::string receivedString;
        receivedString.assign(reinterpret_cast<const char *>(&(rcvBuf[0])), rcvBuf.size());

        cout << "Received message : " << receivedString << endl;

        // Convert to morse
        string track = helpers::convertToMorseTrack(receivedString);

        cout << "Track :\n---------------\n\n" << track << endl;

        // Play the message in morse
        PWMSongParser parser;
        parser.readTrack(track);
        parser.play(1);
    } catch (...) {
        cout << "Nothing to play..." << endl;
    }

    // Make sure to close the remote socket
    close(localSocket);
    close(remoteSocket);

    cout << "Quitting server" << endl;
    return 0;
}