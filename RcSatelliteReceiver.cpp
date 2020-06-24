/*
* RcSatelliteReceiver.cpp
*  Created on: 2019-06-30
*      Author: Doug Krahmer
*
* Satellite receive serial protocol specifications can be found here: https://www.spektrumrc.com/ProdInfo/Files/Remote%20Receiver%20Interfacing%20Rev%20A.pdf
* Only Servo Field 2048 Mode is supported (no support for DSM2/22ms)
*/

//#define DEBUG 1
#include "RcSatelliteReceiver.h"

RcSatelliteReceiver::RcSatelliteReceiver(void)
{
	_RxSerialPort = &Serial;
	_DebugSerialPort = &Serial;

	for (int i = 0; i < MaxChannels; i++) {
		_channelValues[i] = 0;
		_channelFailsafeValues[i] = 0;
	}
}

RcSatelliteReceiver::RcSatelliteReceiver(Stream *rxSerialPort)
{
	setRxSerialPort(rxSerialPort);
	RcSatelliteReceiver();
}

RcSatelliteReceiver::RcSatelliteReceiver(Stream *rxSerialPort, Stream *debugSerialPort)
{
	setRxSerialPort(rxSerialPort);
	setDebugSerialPort(debugSerialPort);
	RcSatelliteReceiver();
}

void RcSatelliteReceiver::setRxSerialPort(Stream *rxSerialPort)
{
	_RxSerialPort = rxSerialPort;
}

void RcSatelliteReceiver::setDebugSerialPort(Stream *streamObject)
{
	_DebugSerialPort = streamObject;
}

void RcSatelliteReceiver::setMode2048(void)
{
	_mask_chanid = MASK_2048_CHANID;
	_mask_sxpos = MASK_2048_SXPOS;
	_rot_chanid = ROT_2048_CHANID;
	MaxChannelValue = CV_2048_MAX;
	MinChannelValue = CV_2048_MIN;
}

void RcSatelliteReceiver::setMode1024(void)
{
	_mask_chanid = MASK_1024_CHANID;
	_mask_sxpos = MASK_1024_SXPOS;
	_rot_chanid = ROT_1024_CHANID;
	MaxChannelValue = CV_2048_MAX / 2;
	MinChannelValue = CV_2048_MIN / 2;
}

void RcSatelliteReceiver::readChannelValues(void)
{
	int startingByteCount = _RxSerialPort->available();
	if (startingByteCount < MessageLength)
		return; // We do not have a full message, nothing to do.

	if (startingByteCount % MessageLength != 0)
	{
		// Bad read...
#ifdef DEBUG
		_DebugSerialPort->print("Bad message byte count: ");
		_DebugSerialPort->print(startingByteCount);
		_DebugSerialPort->println();
#endif
		// Empty the buffer. Hopefully we get a good message next time
		while (_RxSerialPort->available() >= 1)
			_RxSerialPort->read();

		return;
	}

	int bytesRead = 0;

	// Keep looping while there are still whole messages in the buffer
	while (_RxSerialPort->available() >= MessageLength)
	{
		if (bytesRead > 64)
			break; // make sure we don't get stuck in an infinite loop

		// Possible incoming message structs (they are treated the same since we only care about the servo data):
		// typedef stuct
		// {
		//		UINT8 fades;
		//		UINT8 system;
		//		UINT16 servo[7];
		// } INT_REMOTE_STR;
		// ---- or ---
		// typedef struct
		// {
		//		UINT16 fades;
		//		UINT16 servo[7];
		// } EXT_REMOTE_STR;

		for (int i = 0; i < MessageLength; i += 2) // Read the message 1 word (2 bytes) at a time
		{
			int servoData = (_RxSerialPort->read() << 8) | _RxSerialPort->read(); // read 2 bytes and combine into a word
			bytesRead += 2;

			if (i == 0) // This word contains the fades and system bytes
				continue; // We don't really care about these values so let move on 

			int channelNumber = getChannelNumber(servoData);
			if (channelNumber < 0 || channelNumber > MaxChannels)
				continue; // channel number is out of range

			int channelValue = getChannelValue(servoData);
			_channelValues[channelNumber] = channelValue;
		}

		_lastReceiveMillis = millis();
	}

#ifdef DEBUG
	_DebugSerialPort->print("Bytes read: ");
	if (bytesRead < 100) Serial.print('0');
	if (bytesRead < 10) Serial.print('0');
	_DebugSerialPort->print(bytesRead);
	_DebugSerialPort->print("   ");

	_DebugSerialPort->print("Channel values: ");
	for (int i = 0; i < MaxChannels; i++)
	{
		int channelValue = _channelValues[i];
		if (channelValue < 100) Serial.print('0');
		if (channelValue < 10) Serial.print('0');
		_DebugSerialPort->print(channelValue);
		_DebugSerialPort->print(" ");
	}
	_DebugSerialPort->println();
#endif
}

int RcSatelliteReceiver::getChannelNumber(int servoData)
{
	int channelNumber = (servoData & _mask_chanid) >> _rot_chanid; // convert the bits to an int value for the channel
	return channelNumber;
}

int RcSatelliteReceiver::getChannelValue(int servoData)
{

	int channelValue = (servoData & _mask_sxpos); 
	return channelValue;
}

int RcSatelliteReceiver::getChannel(int chanelNumber)
{
	if (getMillisSinceLastReceive() > FailsafeDelayMilliseconds)
		return _channelFailsafeValues[chanelNumber - 1];

	return _channelValues[chanelNumber - 1];
}

int RcSatelliteReceiver::getThr()
{
	return getChannel(1);
}

int RcSatelliteReceiver::getAil()
{
	return getChannel(2);
}

int RcSatelliteReceiver::getEle()
{
	return getChannel(3);
}

int RcSatelliteReceiver::getRud()
{
	return getChannel(4);
}

unsigned long RcSatelliteReceiver::getMillisSinceLastReceive()
{
	return millis() - _lastReceiveMillis;
}

void RcSatelliteReceiver::setChannelFailsafeValue(int channel, int value)
{
	_channelFailsafeValues[channel - 1] = value;
}
