/**
 * @file main.cpp
 * @author Yassine and Hans
 * @date 20 November 2020
 * @brief Main file, parses args and starts the app in the right mode.
 */

#ifdef __x86_64__
    #define CVUI_IMPLEMENTATION
#endif
#include <iostream>
#include "../include/CLI11.hpp"
#include "../include/recordMain.h"
#include "../include/streamMain.h"
#include "../include/morseMain.h"
#ifdef __x86_64__
    #include "../include/watchMain.h"
#endif

using namespace std;
using namespace cv;
using namespace helpers;

#ifdef __x86_64__
    const string modes[] = {"record", "stream", "morse", "watch"};
#else
    const string modes[] = {"record", "stream", "morse"};
#endif

/**
 * \brief Parses app starting agurments and starts the app in the right mode.
 * \param argc Number of arguments
 * \param argv Array of arguments
 * \return Exit status
 */
int main(int argc, char** argv) {
    CLI::App app{"Projet ELE4205"};

    // Common Options
        string mode;
        app.add_option("-m,--mode", mode, "App mode. One of [" + helpers::join(modes, ", ") + "].");

        uint16_t videoResX = 800;
        app.add_option("-x,--xRes", videoResX, "Video X resolution. Default: " + to_string(videoResX) + ".");

        uint16_t videoResY = 600;
        app.add_option("-y,--yRes", videoResY, "Video Y resolution. Default: " + to_string(videoResY) + ".");

    // Record Options
        double videoLength = 5;
        app.add_option("-l,--length", videoLength, "Video length in seconds. Only in 'record' mode. Default: " + to_string(videoLength) + ".");

    // Stream/Watch Options
        u_int port = 3382;
        app.add_option("-p,--port", port, "Streaming port. Only in 'stream' and 'watch' mode. Default: " + to_string(port) + ".");

    #ifdef __x86_64__
    // Watch Options
        string ipAddr = "127.0.0.1";
        app.add_option("-u,--uri", ipAddr, "Streaming endpoint i.e IP. Only in 'watch' mode. Default: " + ipAddr + ".");
    #endif

        CLI11_PARSE(app, argc, argv);

    cout << "Starting in " << mode << " mode..." << endl;
    // Run chosen command
    if (mode == modes[0]) {
        return recordMain(videoResX, videoResY, videoLength);
    } else if (mode == modes[1]) {
        return streamMain(videoResX, videoResY, port);
    } else if (mode == modes[2]) {
        return morseMain(port);
#ifdef __x86_64__
    } else if (mode == modes[3]) {
        return watchMain(ipAddr.c_str(), port);
#endif
    } else {
        cout << app.help();
        return 0;
    }
}
