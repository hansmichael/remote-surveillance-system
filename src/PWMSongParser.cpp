/*
 * PWMSongParser.cpp
 *
 *  Created on: Aug 13, 2015
 *      Author: Lucas Malo Bélanger
 *  Modified on: Nov 8, 2016
 *           by: R. Gourdeau
 */

#include "../include/PWMSongParser.h"

PWMSongParser::PWMSongParser() {
    dutyPath = "/sys/devices/pwm-ctrl.42/duty0";
	frequencyPath = "/sys/devices/pwm-ctrl.42/freq0";
	enablePath = "/sys/devices/pwm-ctrl.42/enable0";

	dutyFile.open(dutyPath.c_str());
    frequencyFile.open(frequencyPath.c_str());
    enableFile.open(enablePath.c_str());

    if (dutyFile.fail() || frequencyFile.fail() || enableFile.fail()) {
        std::cout << "Failed to open pwm" << std::endl;
        isPWMAvailable = false;
    }

    std::cout <<  "[v2] PWMSongParser starting...\nIs PWM available ? " << std::boolalpha << isPWMAvailable << std::endl;

    if (isPWMAvailable) {
        dutyFile << 512;
        enableFile << 1;

        dutyFile.close();
        frequencyFile.close();
        enableFile.close();
    }
}

PWMSongParser::~PWMSongParser() {
    if (isPWMAvailable) {
        enableFile.open(enablePath.c_str());
        enableFile << 0;
        enableFile.close();

        dutyFile.close();
        frequencyFile.close();
    }
}

void PWMSongParser::tone(uint32_t frequency, uint32_t length)
{
    std::cout << "[PWM] Note On!  F : " << frequency << " , L : " << length << std::endl;
    if (isPWMAvailable) {
        enableFile.open(enablePath.c_str());
        enableFile << 1;
        enableFile.close();

        frequencyFile.open(frequencyPath.c_str());
        frequencyFile << std::to_string(frequency);
        frequencyFile.close();
    }
    helpers::msleep(length);

    // Note if off
    std::cout << "[PWM] Note Off! F : " << frequency << " , L : " << length << std::endl;
    if (isPWMAvailable) {
        enableFile.open(enablePath.c_str());
        enableFile << 0;
        enableFile.close();
    }
}
