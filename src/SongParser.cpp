/*
 * SongParser.cpp
 *
 *  Created on: Aug 13, 2015
 *  Author: Lucas Malo Bélanger
 */

#include "../include/SongParser.h"


SongParser::SongParser() {

}

SongParser::~SongParser() {
	// TODO Auto-generated destructor stub
}

void SongParser::readTrack(std::string trackString)
{
	std::string data1, data2, data3, data4, data5, frequency, length, delay;
	_commands.clear();

	command c;

	std::stringstream track;
	track << trackString;
    track.clear();
    track.seekg(0, track.beg);

    while(!track.eof())
	while(!track.eof())
	{
        track >> data1 >> data2 >> data3;
        track.ignore(1);

        track >> data4 >> data5;
        track.ignore(1);

		frequency.assign(data2, 10,3);
		length.assign(data3,7,3);
		delay.assign(data5, 0, 3);

		c.frequency = atoi(frequency.c_str());
		c.delay = atoi(delay.c_str());
		c.length = atoi(length.c_str());

		_commands.push_back(c);
	}
}
const std::vector<SongParser::command>& SongParser::getCommands() const
{
	return _commands;
}
void SongParser::play(float multiplier)
{
	const std::vector<command>& commands = getCommands();
	for(std::vector<command>::const_iterator it = commands.begin(); it != commands.end(); it++)
	{
	    uint32_t len = (float)(it->length) / multiplier;
        uint32_t del = (float)(it->delay) / multiplier;

		tone(it->frequency, len);
		if (del > 0) {
            std::cout << "[SongParser] Delay : " << del << std::endl;
		    helpers::msleep(del);
		}
	}
}

void SongParser::tone(uint32_t frequency, uint32_t length)
{
	std::cout << "[SongParser] Note On!  F : " << frequency << " , L : " << length << std::endl;
    helpers::msleep(length);

	// Note if off
	std::cout << "[SongParser] Note Off! F : " << frequency << " , L : " << length << std::endl;
}
