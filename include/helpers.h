/**
 * @file helpers.h
 * @author Yassine and Hans
 * @date 20 November 2020
 * @brief Defines all sorts of helpers and implements the template functions.
 */

#ifndef PROJET_HELPERS_H
#define PROJET_HELPERS_H

#include <iostream>
#include <time.h>
#include <sstream>
#include <climits>
#include <algorithm>
#include <iomanip>
#include <fstream>
#ifdef __x86_64__
    #include <zbar.h>
#endif
#include <opencv2/opencv.hpp>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using namespace std;

namespace helpers {
    /**
     * \brief Pause the app for a given amount of time in milliseconds
     * \param msecs time in ms
     */
    void msleep(unsigned int msecs);

    /**
     * \brief Get user input and fallback to default value is user input is bad
     * \tparam T Input value type
     * \param description String to print
     * \param defaultValue self-explanatory
     * \return Input value
     */
    template<class T>
    T getUserInputWithFallback(const string &description, T defaultValue) {
        T value = defaultValue;
        cout << description << " [default=" << defaultValue << "] : ";

        if (cin.peek() == '\n') {
            value = defaultValue;
            cin.ignore(INT_MAX, '\n');
            cin.clear();
        } else if (!(cin >> value)) {
            cout << "Input is not valid, using default.";
            value = defaultValue;
        }

        return value;
    };

    /**
     * \brief Get user input
     * \tparam T Input value type
     * \param description String to print
     * \return Input value
     */
    template<class T>
    T getUserInput(const string &description) {
        T value;
        cout << description << " :";

        bool isNewLine = cin.peek() == '\n';
        if (isNewLine || !(cin >> value)) {
            if (isNewLine) {
                cin.ignore(INT_MAX, '\n');
                cin.clear();
            }
            cout << "Input is not valid\nexiting...";
        }

        return value;
    };

    /**
     * \brief Join multiple strings together with a given separator
     * \tparam Iterator Iterator type
     * \param begin Beginning of container
     * \param end End of container
     * \param separator A string to insert between the other strings
     * \return The joined string
     */
    template <typename Iterator>
    string join(Iterator begin, Iterator end, string separator = ".") {
        std::ostringstream o;
        if(begin != end)
        {
            o << *begin++;
            for(;begin != end; ++begin)
                o  << separator << *begin;
        }
        return o.str();
    }

    /**
     * \brief Join multiple strings together with a given separator
     * \tparam Container Container type
     * \param c Container
     * \param separator A string to insert between the other strings
     * \return The joined string
     */
    template <typename Container>
    string join(Container const& c, string separator = ".") {
        using std::begin;
        using std::end;
        return join(begin(c), end(c), separator);
    }

    /**
     * \brief Generates a string with the current time that can be joined with other strings to constitute a filename
     * \return Current time pretty formatted in a string
     */
    string GetCurrentTimeForFileName();

    /**
     * \brief Enable GPIO pin
     * \param pinNumber self-explanatory
     * \param direction "in" or "out"
     */
    void enableGPIO(uint pinNumber, string direction);

    /**
     * \brief Read GPIO pin
     * \param pinNumber self-explanatory
     * \param inverseLogic invert value read
     * \return true or false
     */
    bool readGPIO(uint pinNumber, bool inverseLogic);

    /**
     * \brief Check light sensor to know if there is light.
     * \return true or false
     */
    bool isThereLight();

#ifdef __x86_64__
    /**
     * \brief Read QR code and return data as string
     * \param frame Image with a QR code
     * \return Empty string or Decoded data
     */
    string detectAndDecodeQR(cv::Mat &frame);
#endif

    /**
     * \brief Convert Char to Morse string
     * \param letter char to convert
     * \return Morse string
     */
    string charToMorse(char letter);

    /**
     * \brief Convert sentence to Morse beep track
     * \param sentence Sentence to convert
     * \return beep track
     */
    string convertToMorseTrack(const string sentence);
}
#endif //PROJET_HELPERS_H
