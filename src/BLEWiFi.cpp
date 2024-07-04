#include "BLEWiFi.h"
#include "Arduino.h"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
bool newData = false;
String receivedData;

void MyServerCallbacks::onConnect(BLEServer* pServer) {
  deviceConnected = true;
}

void MyServerCallbacks::onDisconnect(BLEServer* pServer) {
  deviceConnected = false;
}

void MyCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
  receivedData = pCharacteristic->getValue().c_str(); // Convert to String
  newData = true;
}

void setupBLE(){
    BLEDevice::init("ISAAC");   //BLE Device created
    BLEServer *pServer = BLEDevice::createServer();   //BLE Server created
    // Set a callback function to handle events triggered on the BLE Server
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE
                    );
    // Assign a callback function to handle events related to BLE Characteristic
    pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->setValue("Hello World");

    pService->start();

    // Manage advertising settings of BLE device
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);

    // Sets the advertising interval in hexadecimal format
    pAdvertising->setMinPreferred(0x06);  // 3.75 ms
    pAdvertising->setMinPreferred(0x12);  // 11.25 ms
    BLEDevice::startAdvertising();
    Serial.println("Waiting for a client connection to notify...");
}

bool setupWiFi(){
  if (newData) {
    newData = false;
    Serial.println("Received WiFi credentials");
    
    // Assuming the format is "SSID:password"
    int separator = receivedData.indexOf(':');
    String ssid = receivedData.substring(0, separator);
    String password = receivedData.substring(separator + 1);

    // Connect to WiFi
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi connected");
  }

  return 1;
}