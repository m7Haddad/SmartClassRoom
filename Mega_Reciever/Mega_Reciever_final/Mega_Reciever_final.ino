
#include <IRremote.hpp>

const uint8_t ADDR_CONNECTED = 0x30;
const uint8_t ADDR_LIGHT = 0x31;
const uint8_t ADDR_RED   = 0x32;
const uint8_t ADDR_GREEN = 0x33;
const uint8_t ADDR_BLUE  = 0x34;
const uint8_t ADDR_FAN   = 0x35;

const int PIN_CONNECT = 11;
const int PIN_LIGHT = 8;
const int PIN_R     = 6;
const int PIN_G     = 5;
const int PIN_B     = 3;
const int PIN_FAN   = 12;

void setup() {
  Serial.begin(9600);

  pinMode(PIN_LIGHT, OUTPUT);
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_FAN, OUTPUT);

  IrReceiver.begin(2);

  Serial.println("MEGA READY");
}

void loop() {

  if (IrReceiver.decode()) {
    uint8_t addr = IrReceiver.decodedIRData.address;
    uint8_t val  = IrReceiver.decodedIRData.command;

    switch (addr) {
      
      case ADDR_CONNECTED:
        Serial.println("USER CONNECTED");
        for (int i = 0; i < 5; i++) {
          analogWrite(PIN_CONNECT, val);
          delay(200);
          analogWrite(PIN_CONNECT, 0);
          delay(200);           
          }

        break;
      case ADDR_LIGHT:
        analogWrite(PIN_LIGHT, val);
        Serial.print("Lights Brightness: ");
        Serial.println(val);
        break;

      case ADDR_RED:
        analogWrite(PIN_R, val);
        Serial.print("Red(0-255): ");
        Serial.println(val);
        break;

      case ADDR_GREEN:
        analogWrite(PIN_G, val);
        Serial.print("Green(0-255): ");
        Serial.println(val);
        break;

      case ADDR_BLUE:
        analogWrite(PIN_B, val);
        Serial.print("Blue(0-255): ");
        Serial.println(val);
      break;

      case ADDR_FAN:
        analogWrite(PIN_FAN, val);
        if (val==0){
        Serial.println("Fan Level: OFF");
        }else if (val>0 && val<=160){
        Serial.println("Fan Level: LOW");
        }
        else if (val>160 && val<255){
        Serial.println("Fan Level: MEDIUM");
        }
        else if (val==255){
        Serial.println("Fan Level: HIGH");
        }
      break;

      default:
        break;
    }

    IrReceiver.resume();

  }

}
