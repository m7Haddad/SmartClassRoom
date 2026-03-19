#define IR_SEND_PIN 3


#include <ArduinoBLE.h>
#include <IRremote.hpp>


// NEC addresses for each component
const uint8_t ADDR_CONNECTED = 0x30;
const uint8_t ADDR_LIGHT = 0x31;
const uint8_t ADDR_RED   = 0x32;
const uint8_t ADDR_GREEN = 0x33;
const uint8_t ADDR_BLUE  = 0x34;
const uint8_t ADDR_FAN   = 0x35;


const uint8_t FAN_LEVEL_OFF   = 0;
const uint8_t FAN_LEVEL_LOW   = 160;
const uint8_t FAN_LEVEL_MEDIUM   = 200;
const uint8_t FAN_LEVEL_HIGH   = 255;

BLEService userService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEStringCharacteristic userCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214",BLERead | BLEWrite, 20);


struct Season {
  uint8_t light;
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t fan;
};

struct UserProfile {
  const char* id;
  const char* name;
  Season winter;
  Season summer;
  Season spring;
};


UserProfile profiles[] = {
  { "default", "Defualt",
    {255, 100, 130, 255, FAN_LEVEL_OFF},
    {255, 255, 120, 0, FAN_LEVEL_HIGH},
    {255, 0, 255, 127, FAN_LEVEL_MEDIUM}
  },
  { "uvais", "Dr. Uvais",
    {255, 0, 128, 128, FAN_LEVEL_OFF},
    {70, 75, 0, 130, FAN_LEVEL_HIGH},
    {200, 255, 0, 255, FAN_LEVEL_MEDIUM}
  },
  { "mohamad", "Mohamad",
    {185, 127, 255, 0, FAN_LEVEL_LOW},
    {255, 227, 66, 52, FAN_LEVEL_HIGH},
    {170, 223, 115, 255, FAN_LEVEL_HIGH}
  },
  { "abdelaziz", "Abdelaziz",
    {190, 150, 255, 180, FAN_LEVEL_MEDIUM},
    {195, 255, 160, 100, FAN_LEVEL_HIGH},
    {235, 80, 200, 200, FAN_LEVEL_LOW}
  },
  { "moaz", "Moaz",
    {50, 255, 0, 0, FAN_LEVEL_LOW},
    {255, 0, 255, 0, FAN_LEVEL_HIGH},
    {160, 0, 0, 255, FAN_LEVEL_HIGH}
  },
  { "zaid", "Zaid",
    {180, 200, 255, 120, FAN_LEVEL_OFF},
    {245, 128, 0, 128, FAN_LEVEL_MEDIUM},
    {50, 150, 220, 255, FAN_LEVEL_OFF}
  }
};

int NUM_PROFILES = sizeof(profiles) / sizeof(profiles[0]);
int currentUser = 0;

// Sending an NEC packet
void sendPacket(uint8_t address, uint8_t value) {
  IrSender.sendNEC(address, value, 0); // IR transmits
  Serial.print("Sent NEC: ADDR=0x");
  Serial.print(address, HEX);
  Serial.print("  VAL=");
  Serial.println(value);
  delay(150);//500
}


//Sending 5 NEC packets for a PRESET
void sendFullSetting(Season s) {
  sendPacket(ADDR_LIGHT, s.light);
  sendPacket(ADDR_RED,   s.r);
  sendPacket(ADDR_GREEN, s.g);
  sendPacket(ADDR_BLUE,  s.b);
  sendPacket(ADDR_FAN,   s.fan);
}

//Sending 5 NEC packets for a MANUAL
void sendManualValues(uint8_t light, uint8_t r, uint8_t g, uint8_t b, uint8_t fan) {
  sendPacket(ADDR_LIGHT, light);
  sendPacket(ADDR_RED,   r);
  sendPacket(ADDR_GREEN, g);
  sendPacket(ADDR_BLUE,  b);
  sendPacket(ADDR_FAN,   fan);
}


//function is used for bluetooth connection, returns true when selected user is valid
bool setUser(String id) {
  id.trim();
  for (int i = 0; i < NUM_PROFILES; i++) {
    if (id.equals(profiles[i].id)) {
      currentUser = i;
      return true;
    }
  }
  return false;
}



void setup() {
  Serial.begin(9600);

  // IR setuo
  IrSender.begin();

  // BLE setup
  BLE.begin();


  BLE.setDeviceName("PresenceRoomR4");
  userService.addCharacteristic(userCharacteristic);

  BLE.addService(userService);
  BLE.advertise();

}


void loop() {

  BLEDevice central = BLE.central();

  if (central) {
    Serial.println("BLE connected");

    while (central.connected()) {

      if (userCharacteristic.written()) {
        String id = userCharacteristic.value();
        id.trim();

        Serial.print("BLE received: ");
        Serial.println(id);

        // User selection
        if (setUser(id)) {
          Serial.print("Active user: ");
          Serial.println(profiles[currentUser].name);
          continue;
        }

        Serial.println("Invalid user");
        central.disconnect();
        continue; 

      }

      //MATLAB SERIAL
      if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();

        if (cmd == "CONNECTED") 
          sendPacket(ADDR_CONNECTED,255);
        else if (cmd == "WINTER") 
          sendFullSetting(profiles[currentUser].winter);
        else if (cmd == "SUMMER")
          sendFullSetting(profiles[currentUser].summer);
        else if (cmd == "SPRING")
          sendFullSetting(profiles[currentUser].spring);
        else if (cmd.startsWith("MAN")) {
          
          int light_i, r_i, g_i, b_i, fan_i;
          sscanf(cmd.c_str(), "MAN %d %d %d %d %d",&light_i, &r_i, &g_i, &b_i, &fan_i);

          uint8_t light = (uint8_t)light_i;
          uint8_t r     = (uint8_t)r_i;
          uint8_t g     = (uint8_t)g_i;
          uint8_t b     = (uint8_t)b_i;
          uint8_t fan   = (uint8_t)fan_i;

          sendManualValues(light, r, g, b, fan);
}

      }

      delay(20);
    }

    Serial.println("BLE disconnected");
  }
}
