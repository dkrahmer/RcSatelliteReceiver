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
	unsigned long getMillisSinceLastReceive();
	void setChannelFailsafeValue(int channel, int value);
	const int MinChannelValue = 342;
	const int MaxChannelValue = 1706;
	const int MessageLength = 16;
	unsigned long FailsafeDelayMilliseconds = 250; // milliseconds since the last receive to wait before enabling Fail-Safe values

private:
	int getChannelNumber(int servoData);
	int getChannelValue(int servoData);
	const int MaxChannels = 12;
	int _channelValues[12];
	int _channelFailsafeValues[12];
	unsigned long _lastReceiveMillis = 0;
};

#endif // RCSATELLITERECEIVER_H_
