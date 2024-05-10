#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <ESP32Servo.h>
#include <SPI.h>
#include <RFID.h>

#define servoPin1 13
#define servoPin2 14
#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN, RST_PIN);

Servo servo1;
Servo servo2;

const int CUTOFF = -30;
String uuid = "";
bool servo_closed = false;

bool scan_for_ble_proximity() {
  BLEScan *scan = BLEDevice::getScan();
  scan -> setActiveScan(true);
  BLEScanResults results = scan -> start(1);
  
  int best = CUTOFF;

  for (int i = 0; i< results.getCount(); i++) {
    BLEAdvertisedDevice device = results.getDevice(i);
    int rssi = device.getRSSI();
    if (rssi > best) {
      best = rssi;
      uuid = device.getServiceUUID().toString().c_str();
    }
  }

  return best > CUTOFF ? true : false;
}

void close_servo() {
  for (int angle = 0; angle <= 90; angle += 5) {
    servo1.write(angle);
    servo2.write(angle);
    delay(15);
  }
}

void open_servo() {
  for (int angle = 90; angle >= 0; angle -= 5) {
    servo1.write(angle);
    servo2.write(angle);
    delay(15);
  }
}

bool rfid_proximity() {
  if (rfid.isCard()) {
    Serial.println("Find the card!");
    
    return true;
  }
  rfid.halt();
  return false;
}

void setup() {
  BLEDevice::init("Drone");

  servo1.attach(servoPin1);
  servo2.attach(servoPin2);

  SPI.begin();
  rfid.init();

	delay(100);
}

void loop() {
  uuid = "";
  bool near_object = scan_for_ble_proximity();
  bool near_rfid = rfid_proximity();

  if (near_object && uuid == "sasbk" && !servo_closed) {
    close_servo();
    servo_closed = true;
  } else if (near_rfid && servo_closed) {
    open_servo();
    servo_closed = false;
    delay(2000);
  }
}
