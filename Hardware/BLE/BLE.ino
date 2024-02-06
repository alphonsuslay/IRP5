// Declare Libraries //
#include <delays.h>
#include <input.h>
#include "npsoe_iot_kit.h"
#include "dht.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Initialize the DHT sensor //
dht DHT;
#define dht_apin A1 //Set the dht pin

// Define custom SDA and SCL pins // 
#define CUSTOM_SDA 9
#define CUSTOM_SCL 8
Adafruit_MPU6050 mpu; // Initialize MPU6050

DELAYMS_T d;


#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic;
BLECharacteristic* pCharacteristic_2;
BLECharacteristic* pCharacteristic3;
BLECharacteristic* pCharacteristic4;
BLECharacteristic* pCharacteristic5;
BLECharacteristic* pCharacteristic6;
BLECharacteristic* pCharacteristic7;
BLECharacteristic* pCharacteristic8;
BLEDescriptor *pDescr;
BLE2902 *pBLE2902;

bool deviceConnected = false;
bool oldDeviceConnected = false;
float value = 0;
String msg;

//Unique IDs
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR1_UUID          "beb5483e-36e1-4688-b7f5-ea07361b26a8" //Temperature
#define CHAR3_UUID          "e05c45a8-2f20-4977-af93-b4715b1b624e" //Humidity
#define CHAR4_UUID          "ef180192-3763-4960-bae9-ac28c259f856" //Heart Rate
#define CHAR5_UUID          "b6a7dd2e-a6ed-4f2f-b7d8-22e9fe919e1e" //Acceleration
#define CHAR6_UUID          "554a8d00-6ad7-4794-89eb-d1d257c2a93e" //Rotation
#define CHAR7_UUID          "d13fe8d0-d5a1-41f6-b5d0-f4c437014458" //Red Led
#define CHAR8_UUID          "55fed505-7fb7-41b1-9016-a1e35db40bec" //Green Led
#define CHAR2_UUID          "e3223119-9445-4e96-a4a1-85358c4046a2"

// const variables Configuration //
const int sensorMin = 0;   // Minimum raw sensor value
const int sensorMax = 4095; // Maximum raw sensor value
const int heartRateMin = 60;  // Minimum heart rate value
const int heartRateMax = 120 ; // Maximum heart rate value




// Callback functions that get triggered when device connects or disconnects from ESP32
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


// This callback function gets triggered when data is written to CHAR2_UUID.
class CharacteristicCallBack: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pChar) override { 
    std::string pChar2_value_stdstr = pChar->getValue();
    String pChar2_value_string = String(pChar2_value_stdstr.c_str());
    int pChar2_value_int = pChar2_value_string.toInt();
    Serial.println("pChar2: " + String(pChar2_value_int)); 
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(3, OUTPUT);
  ledcSetup(0, 5000, 8);  // Use your preferred values
  ledcAttachPin(6, 0);   // Change pin accordingly
  ledcAttachPin(7, 1);
  BLE_Server_Setup();
  MPU6050Config();
}

void loop() {
    // notify changed value
    if (deviceConnected) {
      Serial.println("Connected to Device");
      DHT.read11(dht_apin);
      value = DHT.humidity;
      msg = (String) value;
      pCharacteristic->setValue(msg.c_str()); // Set value of the BLE Characteristic with the sensor reading
      pCharacteristic->notify(); // Notify connected devices about value 
      Serial.print("Humidity Val: ");
      Serial.println(msg);
      int adcRes = analogRead(A0);
      int heartRate = map(adcRes, sensorMin, sensorMax, heartRateMin, heartRateMax);
      Serial.print("\nRaw Analog value = ");
      Serial.println(adcRes);
      Serial.print("Heart Rate = ");
      Serial.println(heartRate);
      if (heartRate > 90)  { //Control the LEDs based on heart rate value
      digitalWrite(18, HIGH);   // Turn on the red LED
      digitalWrite(19, LOW);
      value = 1;
      msg = (String) value;
      pCharacteristic7->setValue(msg.c_str());
      pCharacteristic7->notify();
      Serial.print(msg);

      }
      else {
      digitalWrite(18, LOW);   // Turn off the red LED
      digitalWrite(19, HIGH);
      value = 0;
      msg = (String) value;
      pCharacteristic7->setValue(msg.c_str());
      pCharacteristic7->notify();

      }
      msg = (String) heartRate;
      pCharacteristic4->setValue(msg.c_str());
      pCharacteristic4->notify();
      value = DHT.temperature;
      msg = (String) value;  
      pCharacteristic3->setValue(msg.c_str());
      pCharacteristic3->notify();
      Serial.print("Temperature Val: ");
      Serial.println(msg);

      
      sensors_event_t a, g, temp; // Read acceleration and rotation from MPU6050
      mpu.getEvent(&a, &g, &temp);


      Serial.print(", Y: ");
      Serial.print(a.acceleration.y);
      msg = (String) a.acceleration.y;
      pCharacteristic5->setValue(msg.c_str());
      pCharacteristic5->notify();
      if (a.acceleration.y<=-1){
        tone(3, 1000);
        
      }
      else{
        noTone(3); 
      }

      

      
      Serial.print(", Y: ");
      Serial.print(g.gyro.y);
      msg = (String) g.gyro.y;
      pCharacteristic6->setValue(msg.c_str());
      pCharacteristic6->notify();




      delay(2000);
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
        digitalWrite(18, LOW);
        digitalWrite(19, LOW);
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

// Setup BLE server
void BLE_Server_Setup() {
  // Create the BLE Device & Set name to be discovered
  BLEDevice::init("phatcat254");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());


  //Creates a BLE service and two characteristics, one for notifications and one for reading and writing.
  BLEService *pService = pServer->createService(SERVICE_UUID);

  //Characteristics for different sensor readings
  pCharacteristic = pService->createCharacteristic(CHAR1_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic3 = pService->createCharacteristic(CHAR3_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic4 = pService->createCharacteristic(CHAR4_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic5 = pService->createCharacteristic(CHAR5_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic6 = pService->createCharacteristic(CHAR6_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic7 = pService->createCharacteristic(CHAR7_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic8 = pService->createCharacteristic(CHAR8_UUID, BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic_2 = pService->createCharacteristic(CHAR2_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);


  //Descriptors provide additional information about a characteristic.
  pDescr = new BLEDescriptor((uint16_t)0x2901);
  pDescr->setValue("A very interesting variable");
  pCharacteristic->addDescriptor(pDescr);
  pBLE2902 = new BLE2902();
  pBLE2902->setNotifications(true);
  pCharacteristic->addDescriptor(pBLE2902);
  pCharacteristic_2->addDescriptor(new BLE2902());
  
  //Assigns the characteristic callback function to CHAR2_UUID.
  pCharacteristic_2->setCallbacks(new CharacteristicCallBack());
  
  // Start the service
  pService->start();

  //Sets up advertising to make the device discoverable by other BLE devices.
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}
//Setup MPU6050 configuration
void MPU6050Config() { 
  Wire.begin(CUSTOM_SDA, CUSTOM_SCL);                         //added
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // Set accelerometer, gyro range and filter bandwidth
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: +-8G");

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: +- 500 deg/s");

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: 5 Hz");

  Serial.println("");
  delay(100);
}
