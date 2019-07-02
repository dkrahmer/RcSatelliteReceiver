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
	for (int i = 0; i < MaxChannels; i++)
		_channelValues[i] = 0;
}

void RcSatelliteReceiver::readChannelValues(void)
{
	int startingByteCount = Serial.available();
	if (startingByteCount < MessageLength)
		return; // We do not have a full message, nothing to do.

	if (startingByteCount % MessageLength != 0)
	{
		// Bad read...
#ifdef DEBUG
		Serial.print("Bad message byte count: ");
		Serial.print(startingByteCount);
		Serial.println();
#endif
		// Empty the buffer. Hopefully we get a good message next time
		while (Serial.available() >= 1)
			Serial.read();

		return;
	}

	int bytesRead = 0;

	// Keep looping while there are still whole messages in the buffer
	while (Serial.available() >= MessageLength)
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
			int servoData = (Serial.read() << 8) | Serial.read(); // read 2 bytes and combine into a word
			bytesRead += 2;

			if (i == 0) // This word contains the fades and system bytes
				continue; // We don't really care about these values so let move on 

			int channelNumber = getChannelNumber(servoData);
			if (channelNumber < 0 || channelNumber > MaxChannels)
				continue; // channel number is out of range

			int channelValue = getChannelValue(servoData);
			_channelValues[channelNumber] = channelValue;
		}
	}

#ifdef DEBUG
	Serial.print("Bytes read: ");
	if (bytesRead < 100) Serial.print('0');
	if (bytesRead < 10) Serial.print('0');
	Serial.print(bytesRead);
	Serial.print("   ");

	Serial.print("Channel values: ");
	for (int i = 0; i < MaxChannels; i++)
	{
		int channelValue = _channelValues[i];
		if (channelValue < 100) Serial.print('0');
		if (channelValue < 10) Serial.print('0');
		Serial.print(channelValue);
		Serial.print(" ");
	}
	Serial.println();
#endif
}

int RcSatelliteReceiver::getChannelNumber(int servoData)
{
	uint8_t highByte = highByte(servoData);
	const int CHANNEL_NUMBER_MASK = B01111000;
	int channelNumber = (highByte & CHANNEL_NUMBER_MASK) >> 3; // convert the 4 bits to an int value for the channel
	return channelNumber;
}

int RcSatelliteReceiver::getChannelValue(int servoData)
{
	const int HIGHBYTE_CHANNEL_VALUE_MASK = B00000111;
	uint8_t highByte = highByte(servoData);
	uint8_t lowByte = lowByte(servoData);
	int channelValue = (((int)highByte & HIGHBYTE_CHANNEL_VALUE_MASK) << 8) | (int)lowByte; // 342 is the lowest value so we adjust to make it 0
	return channelValue;
}

int RcSatelliteReceiver::getChannel(int chanelNumber)
{
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
