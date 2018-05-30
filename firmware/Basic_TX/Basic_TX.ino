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

const static uint8_t RADIO_ID = 1;             // Our radio's id.
const static uint8_t DESTINATION_RADIO_ID = 255; // Id of the radio we will transmit to.
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
  pinMode(A1, OUTPUT);
  pinMode(A2, INPUT);
  pinMode(A3, OUTPUT);
  digitalWrite(A1, HIGH);
  digitalWrite(A3, LOW);
  Serial.begin(115200);

  if (!_radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN))
  {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }

  _radioData.FromRadioId = RADIO_ID;
}

float movingAverage = 0;
int state = 0;
int lastTime = 0;

int sequence = 0;
int cursor = 100;

byte dataRec = 0;

void loop()
{
  _radioData.OnTimeMillis = millis();
  //
  //    Serial.print("Sending ");
  //    Serial.print(_radioData.OnTimeMillis);
  //    Serial.print(" ms");
  //
  //    if (_radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData))) // Note how '&' must be placed in front of the variable name.
  //    {
  //        Serial.println("...Success");
  //    }
  //    else
  //    {
  //        Serial.println("...Failed");
  //        _radioData.FailedTxCount++;
  //    }

  float samples = 5;
  float total = 0;
  for (int i = 0; i < samples; i++) {
    total += analogRead(A2);
  }
  total /= samples;
  movingAverage = movingAverage + (total - movingAverage) / 13;
  //Serial.println((int)movingAverage);

  int nstate = state;
  if(total - movingAverage > 25) {
    nstate = 1;  
  } else if(total - movingAverage < -25) {
    nstate = 0;
  }
  if (nstate == 1 && state == 0) {
    int interval = millis() - lastTime;
    lastTime = millis();
    //Serial.println(interval);
    int st = 9;
    if (interval < 40) {
      //Serial.print("0");
      st = 0;
    } else if (interval < 60) {
      //Serial.print("1");
      st = 1;
    }else if (interval < 150) {
      //Serial.print("C");
      st = 2;
    }

    if(cursor < 8 && st == 2) {
      cursor = 100;
    } else if(cursor < 7) {
      if(st == 1) {
        dataRec |= (st << cursor);
      }
      cursor++;  
    } else if(cursor == 7) {
      int parity = 0; 
      for(int i = 0; i < 8; i++) {
        if((1 << i) & dataRec != 0) {
          parity++;  
        }
      }
      if((parity % 2 == 0 && st == 1)
      || (parity % 2 != 0 && st == 0)) {
        Serial.print("DATA GOOD: ");
        Serial.println(dataRec);
      } else {
        Serial.println("BAD DATA");  
        Serial.println(dataRec);
      }
      cursor = 100;
    }

    if((sequence < 3) && st == 2) {
      sequence++;  
    }
    else {
      sequence = 0;  
    }

    if(sequence == 3) {
      Serial.println("PREM");
      cursor = 0; 
      dataRec = 0;
      sequence = 0;
    }
  }
  state = nstate;
  //    Serial.print(",");
  //    Serial.println(total > movingAverage ? 1 : 0);

  /*
    By default, 'send' transmits data and waits for an acknowledgement.
    You can also send without requesting an acknowledgement.
    _radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData), NRFLite::NO_ACK)
    _radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData), NRFLite::REQUIRE_ACK) // THE DEFAULT
  */
}
