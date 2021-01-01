/*
 * PWMSongParser.h
 *
 * Created on: Aug 13, 2015
 * Author: Lucas Malo Bélanger
 */

#ifndef PWMSONGPARSER_H_
#define PWMSONGPARSER_H_

#include "SongParser.h"

/**
 * PWM Song parser and player
 * Implements hardware buzzer
 */
class PWMSongParser: public SongParser{
public:
    /**
     * \brief Inits the default PWM song parser
     */
    PWMSongParser();

    /**
     * \brief Destroys PWM song parser object
     */
    virtual ~PWMSongParser();

	std::string frequencyPath; /**< File path to the file that holds the frequency */
	std::string enablePath; /**< File path to the file that holds the PWM status */
    std::string dutyPath; /**< File path to the file that holds the duty cycle */

    bool isPWMAvailable; /**< Is set to true when hardware PWM is available */

    std::ofstream dutyFile; /**< File that holds the frequency */
    std::ofstream enableFile; /**< File that holds the PWM status */
    std::ofstream frequencyFile; /**< File that holds the duty cycle */
private:
	/**
	 * \brief Plays the Tone, overridden to play tone on hardware
	 * \param frequency The frequency in Hz
	 * \param length The length in ms
	 */
    virtual void tone(uint32_t frequency, uint32_t length);
};

#endif /* PWMSONGPARSER_H_ */
