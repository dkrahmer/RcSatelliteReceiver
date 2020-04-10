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
	RcSatelliteReceiver(Stream *rxSerialPort);
	RcSatelliteReceiver(Stream *rxSerialPort, Stream *debugSerialPort);
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
	const int MASK_1024_CHANID = 0xFC00;
	const int ROT_1024_CHANID = 10;
	const int MASK_1024_SXPOS = 0x03FF;
	const int MASK_2048_CHANID = 0x7800;
	const int ROT_2048_CHANID = 11;
	const int MASK_2048_SXPOS = 0x07FF;
	unsigned long FailsafeDelayMilliseconds = 250; // milliseconds since the last receive to wait before enabling Fail-Safe values
	void setRxSerialPort(Stream *rxSerialPort);
	void setDebugSerialPort(Stream *debugSerialPort);
	void setMode1024();
	void setMode2048();

private:
	int getChannelNumber(int servoData);
	int getChannelValue(int servoData);
	const int MaxChannels = 12;
	int _channelValues[12];
	int _channelFailsafeValues[12];
	unsigned long _lastReceiveMillis = 0;
	Stream *_RxSerialPort;
	Stream *_DebugSerialPort;
	int _mask_chanid = MASK_2048_CHANID;
	int _rot_chanid = ROT_2048_CHANID;
	int _mask_sxpos = MASK_2048_SXPOS;
};

#endif // RCSATELLITERECEIVER_H_
