#include <BLEDevice.h>

#define SERVICE_UUID  "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

BLEAdvertising *pAdvertising = NULL;
BLEServer* pServer = NULL;

void setup() {
  ///////////////////////////// Setting Up BLE Beacon ////////////////////////////////

  // Create the BLE Device
  BLEDevice::init("BLE_OBJECT_1");

  // Create the BLE Server
  pServer = BLEDevice::createServer();

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Start the service
  pService->start();

  // Start advertising
  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
}

void loop() {
  // Your main code loop
}
