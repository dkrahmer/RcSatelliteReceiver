/*
 * RcSatelliteReceiver.h
 *  Created on: 2019-06-30
 *      Author: Doug Krahmer
 */

#ifndef RCSATELLITERECEIVER_H_
#define RCSATELLITERECEIVER_H_

#include "Arduino.h"

class RcSatelliteReceiver {
public:
	RcSatelliteReceiver(void);
	void readChannelValues(void);
	int getChannel(int chanelNumber);
	int getThr();
	int getAil();
	int getEle();
	int getRud();
	const int MinChannelValue = 342;
	const int MaxChannelValue = 1706;
	const int MessageLength = 16;

private:
	int getChannelNumber(int servoData);
	int getChannelValue(int servoData);
	const int MaxChannels = 12;
	int _channelValues[12];
};

#endif // RCSATELLITERECEIVER_H_