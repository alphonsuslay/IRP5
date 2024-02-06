// Arduino Declarations //

#include <delays.h>
#include <WiFi.h>
#include <input.h>
#include "npsoe_iot_kit.h"
#include "dht.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Initialize DHT sensor and MPU6050 accelerometer/gyroscope //
dht DHT;
Adafruit_MPU6050 mpu;

// Initialize delay and wifi //
DELAYMS_T d;
WiFiClient wifiClient;

// Define pin configurations for DHT sensor and custom SDA/SCL pins // 
#define dht_apin A1
#define CUSTOM_SDA 9
#define CUSTOM_SCL 8

// Firebase Declarations //

#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Firebase authentication and configuration //
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Key in own API key and own database url // 
#define API_KEY ""
#define DATABASE_URL ""

// Wifi Configuration //
const char* ssid = "";
const char* password = "";

// Topic Configuration //
const char *HeartRateTopic = "Sensor/Rotary";
const char *dhtTemperatureTopic = "DHT/Temperature";
const char *dhtHumidityTopic = "DHT/Humidity";
const char *ledRedTopic = "LED/Red";
const char *ledGreenTopic = "LED/Green";
const char *PushButtonTopic = "Button/PB";
const char *mpu6050AccelXTopic = "MPU6050/Acceleration/X";
const char *mpu6050AccelYTopic = "MPU6050/Acceleration/Y";
const char *mpu6050AccelZTopic = "MPU6050/Acceleration/Z";
const char *mpu6050RotXTopic = "MPU6050/Rotation/X";
const char *mpu6050RotYTopic = "MPU6050/Rotation/Y";
const char *mpu6050RotZTopic = "MPU6050/Rotation/Z";


// const variables Configuration //
const int sensorMin = 0;   // Minimum raw sensor value
const int sensorMax = 4095; // Maximum raw sensor value
const int heartRateMin = 80;  // Minimum heart rate value
const int heartRateMax = 105 ; // Maximum heart rate value
int redLEDState = LOW;
int greenLEDState = LOW;
bool SignUpOK = false;
bool SystemState = true;

int PbState = 0;
int oldFirebasePbState = 0;
int FirebasePBState = 0;
int currentstate = 0;


INPUT_T pb1(4, INPUT_PULLUP, 50, PB1_ON_VAL);


// Function to check push button state // 
void chkPB1_nonBlocking() {
  if (pb1.justPressed()) {
    Serial.print("\nPB1 pressed ");
    PbState = !PbState;
    PublishToFirebase(PushButtonTopic, PbState);
  }
}


void setup() {
  Serial.begin(115200);
  delay(500);
  d.dlySet(1000);

  pinMode(18, OUTPUT); // Red LED
  pinMode(19, OUTPUT); // Green LED
  pinMode(3, OUTPUT);
  ledcSetup(0, 5000, 8); // Use your preferred values
  ledcAttachPin(6, 0);   // Change pin accordingly
  ledcAttachPin(7, 1); 

  // Connect to wifi and firebase //
  connectToWiFi();
  connectToFirebase();
  
  MPU6050Config();
}

void loop() {
  chkPB1_nonBlocking();
  int FirebasePBState;
  FirebasePBState = GetFromFirebase(PushButtonTopic);
  
  // Update oldFirebasePbState if there's a change in Firebase state
  if (oldFirebasePbState != FirebasePBState) {
    oldFirebasePbState = FirebasePBState;
    // If Firebase state changes, update local state
    PbState = FirebasePBState;
    currentstate = FirebasePBState;
  }

  if (currentstate == 1) { //Check to see current state of PB, 
    if (d.dlyExpiredRestart()) {
    digitalWrite(18, !digitalRead(18));
    PublishHeartRate();
    PublishDHT();
    PublishMPU6050();
    }
  }
  else {
    digitalWrite(18, LOW);   // Turn off the red LED
    digitalWrite(19, LOW);
    greenLEDState = LOW;
    redLEDState = LOW;
    PublishToFirebase(ledRedTopic, redLEDState);
    PublishToFirebase(ledGreenTopic, greenLEDState);
  }
}

// Publish Codes //
void PublishDHT() {
  DHT.read11(dht_apin); // Read temperature and humidity values from dht sensor
  PublishToFirebase(dhtTemperatureTopic, DHT.temperature);
  PublishToFirebase(dhtHumidityTopic, DHT.humidity);
  Serial.print("\n\nPOST: Humidity - ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("\nPOST: Temperature - ");
  Serial.print(DHT.temperature);
  Serial.println("C  ");
}

void PublishHeartRate() {
  int adcRes = analogRead(A0); // Read sesnor and map to heart rate
  int heartRate = map(adcRes, sensorMin, sensorMax, heartRateMin, heartRateMax);
  

  PublishToFirebase(HeartRateTopic, heartRate);// Publish heart rate to firebase
  Serial.print("\n\nPOST: Heart Rate - ");
  Serial.println(heartRate);
  if (heartRate > 100) { //Control the LEDs based on heart rate value
    digitalWrite(18, HIGH);   // Turn on the red LED
    digitalWrite(19, LOW);
    redLEDState = HIGH;
    greenLEDState = LOW;
    PublishToFirebase(ledRedTopic, redLEDState);
    PublishToFirebase(ledGreenTopic, greenLEDState);
  } else {
    digitalWrite(18, LOW);   // Turn off the red LED
    digitalWrite(19, HIGH);
    greenLEDState = HIGH;
    redLEDState = LOW;
    PublishToFirebase(ledRedTopic, redLEDState);
    PublishToFirebase(ledGreenTopic, greenLEDState);
  }
}

void PublishMPU6050() {
  sensors_event_t a, g, temp; // Read acceleration and rotation from MPU6050
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  PublishToFirebase(mpu6050AccelXTopic, a.acceleration.x);
  Serial.print("\n\nAcceleration X: ");
  Serial.print(a.acceleration.x);

  PublishToFirebase(mpu6050AccelYTopic, a.acceleration.y);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  if(a.acceleration.y<0){
    tone(3, 1000);
    
  }
  else{
    noTone(3);
  }

  PublishToFirebase(mpu6050AccelZTopic, a.acceleration.z);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  
  PublishToFirebase(mpu6050RotXTopic, g.gyro.x);
  Serial.print("\n\nRotation X: ");
  Serial.print(g.gyro.x);
  
  PublishToFirebase(mpu6050RotYTopic, g.gyro.y);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);

  PublishToFirebase(mpu6050RotZTopic, g.gyro.z);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");
}


// Setup //
void PublishToFirebase(const char* topic, int value) { //Takes the topic and sensor value to publish to database
  if (Firebase.ready() && SignUpOK) {
    if (Firebase.RTDB.setInt(&fbdo, topic, value)) {
      Serial.println();
      Serial.print(value);
      Serial.print(" - Successfully saved to " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ") ");
    } else {
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  }
}

int GetFromFirebase(const char* topic) { //Get values from firebase database
  if (Firebase.ready() && SignUpOK) {
    if (Firebase.RTDB.getInt(&fbdo, topic)) {
      if (fbdo.dataType() == "int") {
        int value;
        value = fbdo.intData();
        Serial.println();
        Serial.print(value);
        Serial.print(" - Successfully GET from " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ") ");
          return value;
      }
      else {
        Serial.println("FAILED (NOT INT): " + fbdo.errorReason());
        return -1;
      }
    } else {
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  } else {
    Serial.println("Firebase not ready or SignUp failed");
  }
}

void connectToFirebase() {
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "33gdfddafaSFssss3@outlook.com", "MkDpI9m22")) {// Use email and password to singup for authentication 
    Serial.println("Sign Up OK");
    SignUpOK = true;
  } else {
    Serial.printf("Sign Up failed: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void connectToWiFi() {
  Serial.println("\nConnecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}
//Setup MPU6050 configuration // 
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
  Serial.println("Accelerometer range set to: +-8G");

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.println("Gyro range set to: +- 500 deg/s");

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.println("Filter bandwidth set to: 5 Hz");

  Serial.println("");
  delay(100);
}
