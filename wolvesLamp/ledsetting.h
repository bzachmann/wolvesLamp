/*
 * LEDSetting.h
 *
 *  Created on: Aug 10, 2017
 *      Author: Boston
 */

#ifndef LEDSETTING_H_
#define LEDSETTING_H_

#include <stdint.h>

#define DEFAULT_CT_PROGRESS     (0)
#define DEFAULT_CT_STATE        (true)

class LEDSetting
{
private:
	enum color_t
	{
		COLOR_RED,
		COLOR_GREEN,
		COLOR_BLUE,
	};

public:
    LEDSetting() : progress(DEFAULT_CT_PROGRESS), state(DEFAULT_CT_STATE) {}

	void setState(bool const &value);
    void setProgress(uint8_t const &value);
    void getRGB(uint8_t &r, uint8_t &g, uint8_t &b);

private:
    uint8_t progress;
	bool state;
};

#endif /* LEDSETTING_H_ */
