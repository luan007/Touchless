/*

Demonstrates simple RX and TX operation.
Please read the notes in NRFLite.h for a description of all library features.

Radio    Arduino
CE    -> 9
CSN   -> 10 (Hardware SPI SS)
MOSI  -> 11 (Hardware SPI MOSI)
MISO  -> 12 (Hardware SPI MISO)
SCK   -> 13 (Hardware SPI SCK)
IRQ   -> No connection
VCC   -> No more than 3.6 volts
GND   -> GND

*/

#include <SPI.h>
#include <NRFLite.h>

const static uint8_t RADIO_ID = 255;       // Our radio's id.  The transmitter will send to this id.
const static uint8_t PIN_RADIO_CE = 7;
const static uint8_t PIN_RADIO_CSN = 8;

struct RadioPacket // Any packet up to 32 bytes can be sent.
{
    uint8_t FromRadioId;
    uint32_t OnTimeMillis;
    uint32_t FailedTxCount;
};

NRFLite _radio;
RadioPacket _radioData;

void setup()
{
    Serial.begin(115200);

    if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
    {
        Serial.println("Cannot communicate with radio");
        while (1); // Wait here forever.
    }
    
    /*
    By default, 'init' configures the radio to use a 2MBPS bitrate on channel 100 (channels 0-125 are valid).
    Both the RX and TX radios must have the same bitrate and channel to communicate with each other.
    You can assign a different bitrate and channel as shown below.
    _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE250KBPS, 0)
    _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE1MBPS, 75)
    _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE2MBPS, 100) // THE DEFAULT
    */
}

void loop()
{
    while (_radio.hasData())
    {
        _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.

        String msg = "Radio ";
        msg += _radioData.FromRadioId;
        msg += ", ";
        msg += _radioData.OnTimeMillis;
        msg += " ms, ";
        msg += _radioData.FailedTxCount;
        msg += " Failed TX";

        Serial.println(msg);
    }
}