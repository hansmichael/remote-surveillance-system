/*
 * SongParser.h
 *
 *  Created on: Aug 13, 2015
 *      Author: Lucas Malo Bélanger
 */

#ifndef SONGPARSER_H_
#define SONGPARSER_H_

#include <string>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include "helpers.h"

/**
 * Song parser and player
 * Only prints to stdout, this class needs to be inherited
 */
class SongParser {
public:
    /**
     * \brief Inits the default song parser
     */
	SongParser();

    /**
     * \brief Destroys song parser object
     */
	virtual ~SongParser();

	/**
	 * \brief Parse track into command vector
	 * \param trackString
	 */
	void readTrack(std::string trackString);
	
	/**
	 * \brief Play music
	 */
	virtual void play(float);

private:
    /**
     * Command
     */
	struct command {
		uint32_t frequency; /**< Command's Frequency */
		uint32_t length; /**< Command's length */
		uint32_t delay; /**< Command's delay */
	};
	
	std::vector<command> _commands; /**< List of commands that make up a music */
protected:

    /**
     * Commands getter
     * \return commands vector
     */
	const std::vector<command>& getCommands() const;

    /**
     * \brief Plays the Tone, overridde this to play tone in another way
     * \param frequency The frequency in Hz
     * \param length The length in ms
     */
	virtual void tone(uint32_t frequency, uint32_t length);
};

#endif /* SONGPARSER_H_ */
