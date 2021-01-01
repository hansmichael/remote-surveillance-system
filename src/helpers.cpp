/**
 * @file helpers.cpp
 * @author Yassine and Hans
 * @date 20 November 2020
 * @brief Implements all sorts of helpers functions.
 */

#ifndef PROJET_HELPERS_C
#define PROJET_HELPERS_C

#include "../include/helpers.h"

using namespace std;

void helpers::msleep(unsigned int msecs) {
    std::chrono::milliseconds timespan(msecs);
    std::this_thread::sleep_for(timespan);
}

string helpers::GetCurrentTimeForFileName() {
    auto time = std::time(nullptr);
    stringstream ss;
    ss << put_time(std::localtime(&time), "%F_%T"); // ISO 8601 without timezone information.
    auto s = ss.str();
    replace(s.begin(), s.end(), ':', '-');
    return s;
}

void helpers::enableGPIO(uint pinNumber, string direction = "in") {
    if (direction != "in" && direction != "out") {
        direction = "in";
    }

    ofstream writeOnlyFile;
    string pinNumberStr = to_string(pinNumber);

    // echo pinNumber > /sys/class/gpio/export
    writeOnlyFile.open("/sys/class/gpio/export");
    writeOnlyFile << pinNumberStr;
    writeOnlyFile.close();

    // echo in > /sys/class/gpio/gpio{pinNumber}/direction
    writeOnlyFile.open("/sys/class/gpio/gpio" + pinNumberStr + "/direction");
    writeOnlyFile << direction;
    writeOnlyFile.close();
}

bool helpers::readGPIO(uint pinNumber, bool inverseLogic) {
    int pinValue = inverseLogic;
    ifstream readOnlyFile;
    string pinNumberStr = to_string(pinNumber);

    // cat /sys/class/gpio/gpio{pinNumber}/value
    readOnlyFile.open("/sys/class/gpio/gpio" + pinNumberStr + "/value");
    readOnlyFile >> pinValue;
    readOnlyFile.close();

    return inverseLogic ? pinValue == 0 : pinValue == 1;
}

bool helpers::isThereLight() {
    int lightValue = 1019;
    ifstream readOnlyFile;

    // Read light value
    readOnlyFile.open("/sys/class/saradc/ch0");
    readOnlyFile >> lightValue;
    readOnlyFile.close();

    return lightValue < 1020;
}

#ifdef __x86_64__
string helpers::detectAndDecodeQR(cv::Mat &frame) {
    zbar::ImageScanner scanner;
    scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

    cv::Mat greyFrame;
    cvtColor(frame, greyFrame,CV_BGR2GRAY);

    zbar::Image image(greyFrame.cols, greyFrame.rows, "Y800", (uchar *)greyFrame.data, greyFrame.cols * greyFrame.rows);

    int nResults = scanner.scan(image);
    if (nResults < 1) return "";

    return image.symbol_begin()->get_data();
}
#endif

string helpers::charToMorse(char letter) {
    switch(letter)
    {
        case 'a': case 'A': return ".-"; break;
        case 'b': case 'B': return "-..."; break;
        case 'c': case 'C': return "-.-."; break;
        case 'd': case 'D': return "-.."; break;
        case 'e': case 'E': return "."; break;
        case 'f': case 'F': return "..-."; break;
        case 'g': case 'G': return "--."; break;
        case 'h': case 'H': return "...."; break;
        case 'i': case 'I': return ".."; break;
        case 'j': case 'J': return ".---"; break;
        case 'k': case 'K': return "-.-"; break;
        case 'l': case 'L': return ".-.."; break;
        case 'm': case 'M': return "--"; break;
        case 'n': case 'N': return "-."; break;
        case 'o': case 'O': return "---"; break;
        case 'p': case 'P': return ".--."; break;
        case 'q': case 'Q': return "--.-"; break;
        case 'r': case 'R': return ".-."; break;
        case 's': case 'S': return "..."; break;
        case 't': case 'T': return "-"; break;
        case 'u': case 'U': return "..-"; break;
        case 'v': case 'V': return "...-"; break;
        case 'w': case 'W': return ".--"; break;
        case 'x': case 'X': return "-..-"; break;
        case 'y': case 'Y': return "-.--"; break;
        case 'z': case 'Z': return "--.."; break;
        default: return " "; break;
    }
}

string helpers::convertToMorseTrack(const string sentence) {
    string track = "";

    for (char currentChar : sentence) {
        // Pour chaque lettre
        string morseChar = charToMorse(currentChar);

        for (string::size_type i = 0; i < morseChar.size(); i++) {
            // Pour chaque symbol dans une lettre
            char currentMorseSymbol = morseChar[i];
            char nextMorseSymbol;

            if (i == (morseChar.size() - 1)) {
                nextMorseSymbol = '\0';
            } else {
                nextMorseSymbol = morseChar[i + 1];
            }

            int frequency = 698;
            int dotLengthInMs = 60;

            int lengthInMs = 0;
            int delayInMs = 0;

            if (nextMorseSymbol != '\0') {
                delayInMs = dotLengthInMs;
            } else {
                delayInMs = 3 * dotLengthInMs;
            }

            switch (currentMorseSymbol) {
                case '.':
                    lengthInMs = dotLengthInMs;
                    break;
                case '-':
                    lengthInMs = 3 * dotLengthInMs;
                    break;
                case ' ':
                    delayInMs = 7 * dotLengthInMs;
                    break;
                default:
                    break;
            }

            track += ":beep frequency=" + to_string(frequency) + " length=" + to_string(lengthInMs) + "ms;\n";
            track += ":delay " + to_string(delayInMs) + "ms;\n";
        }

        track += "\n";
    }

    return track.substr(0, track.size() - 2);
}

#endif