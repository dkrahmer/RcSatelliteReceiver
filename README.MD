# RxSatelliteReceiver

## About
The RcSatelliteReceiver Arduino library allows a developer to use a Spektrum-compatible RC satellite receiver as a PRIMARY receiver connected directly to an Arduino (See below for connection information).
Using an RC satellite receiver has many advantages over a standard RC receiver that outputs a PPM/PWM signal, which is an analog signal masquerading as a digital signal. The signal from an RC satellite receiver is digital serial data which is more reliable and contains ZERO jitter. Any jitter observed using this library is not from the protocol, as with PPM, but from the transmitter. Another huge advantage of using an RC satellite receiver directly is that they support up to 12 channels if your transmitter is capable.
Generic RC satellite receivers can be found on the internet for less than $10 and provide all 12 channels. Combine this with a Devo transmitter running [deviation](https://www.deviationtx.com/) firmware and a high quality 12 channel RC configuration can be created very inexpensively.

## Connecting an RC satellite receiver to an Arduino
One of the connectors will need to be removed from the satellite receiver cable to attach the wires to an Arduino. Keep in mind that Arduino sketches are uploaded via the serial pins so the signal pin will need to be disconnected to upload your sketch. Nothing will break if the signal pin is left connected, the sketch will just not upload.

| RC Satellite Wire   | Arduino       | Purpose                     |
|---------------------|---------------|-----------------------------|
| Orange              | 3.3V out      | +3.3V (~20ma)               |
| Black               | Ground        | Ground                      |
| Gray                | Pin 0 (RX in) | Signal (115200 baud serial) |

## Pairing the RC satellite receiver
Be sure to get a satellite receiver that has a pairing button.
1. Hold down the button then apply 3.3V power.
2. Ensure the receiver light is flashing rapidly.
3. Select the BIND option on a DSMX/DSM2 transmitter.
4. Ensure the receiver light is on solid.
5. Done!

## Resources
The serial protocol specifications used to create this library can be found here: [Spektrum Remote Receiver Interfacing Rev A](https://www.spektrumrc.com/ProdInfo/Files/Remote%20Receiver%20Interfacing%20Rev%20A.pdf)

## What Else?
Let me know if you find this library useful. If you find a bug or have an improvement please no not hesitate to let me know or send me a pull request.

Enjoy!

-Doug Krahmer