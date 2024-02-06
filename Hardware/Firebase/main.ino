// Arduino Declarations //

#include <delays.h>
#include <WiFi.h>
#include <input.h>
#include "npsoe_iot_kit.h"
#include "dht.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

dht DHT;
Adafruit_MPU6050 mpu;
DELAYMS_T d;
WiFiClient wifiClient;

#define dht_apin A1
#define CUSTOM_SDA 9
#define CUSTOM_SCL 8

// Firebase Declarations

#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define API_KEY "AIzaSyDz3bJ45Y90rw9HGFo4PyB7BDsIXs5J5co"
#define DATABASE_URL "https://irp-fyp-project-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Wifi Configuration //
const char *ssid = "SINGTEL-2ECB";
const char *password = "8CNHB4YVtk6eWWs";

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
const int sensorMin = 0;      // Minimum raw sensor value
const int sensorMax = 4095;   // Maximum raw sensor value
const int heartRateMin = 60;  // Minimum heart rate value
const int heartRateMax = 120; // Maximum heart rate value
int redLEDState = LOW;
int greenLEDState = LOW;
bool SignUpOK = false;
bool SystemState = true;

int PbState = 0;
int oldFirebasePbState = 0;
int FirebasePBState = 0;
int currentstate = 0;

INPUT_T pb1(4, INPUT_PULLUP, 50, PB1_ON_VAL);

// Non Blocking code to check if Pushbutton is pressed
void chkPB1_nonBlocking()
{
  if (pb1.justPressed())
  {
    Serial.print("\nPB1 pressed ");
    PbState = !PbState;
    PublishToFirebase(PushButtonTopic, PbState);
  }
}

// Setup
void setup()
{
  Serial.begin(115200);
  delay(500);
  d.dlySet(2000);

  pinMode(18, OUTPUT); // Red LED
  pinMode(19, OUTPUT); // Green LED
  pinMode(3, OUTPUT);

  connectToWiFi();
  connectToFirebase();
  MPU6050Config();
}

void loop()
{
  chkPB1_nonBlocking();
  int FirebasePBState;
  FirebasePBState = GetFromFirebase(PushButtonTopic);

  // Update oldFirebasePbState if there's a change in Firebase state
  if (oldFirebasePbState != FirebasePBState)
  {
    oldFirebasePbState = FirebasePBState;
    // If Firebase state changes, update local state
    PbState = FirebasePBState;
    currentstate = FirebasePBState;
  }

  if (currentstate == 1)
  {
    if (d.dlyExpiredRestart())
    {
      digitalWrite(18, !digitalRead(18));
      PublishHeartRate();
      PublishDHT();
      PublishMPU6050();
    }
  }
  else
  {
    digitalWrite(18, LOW);
    digitalWrite(19, LOW);
    greenLEDState = LOW;
    redLEDState = LOW;
    PublishToFirebase(ledRedTopic, redLEDState);
    PublishToFirebase(ledGreenTopic, greenLEDState);
  }
}

// Publish Codes //
void PublishDHT()
{
  DHT.read11(dht_apin);
  Serial.println("\n\nPOST: Humidity - ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("\nPOST: Temperature - ");
  Serial.print(DHT.temperature);
  Serial.println("C  ");
  PublishToFirebase(dhtTemperatureTopic, DHT.temperature);
  PublishToFirebase(dhtHumidityTopic, DHT.humidity);
}

void PublishHeartRate()
{
  int adcRes = analogRead(A0);
  int heartRate = map(adcRes, sensorMin, sensorMax, heartRateMin, heartRateMax);

  Serial.print("\n\nPOST: Heart Rate - ");
  Serial.println(heartRate);

  PublishToFirebase(HeartRateTopic, heartRate);
  if (heartRate > 100 || heartRate < 60)
  {
    digitalWrite(18, HIGH); // Turn on the red LED
    digitalWrite(19, LOW);
    redLEDState = HIGH;
    greenLEDState = LOW;
    PublishToFirebase(ledRedTopic, redLEDState);
    PublishToFirebase(ledGreenTopic, greenLEDState);
  }
  else
  {
    digitalWrite(18, LOW); // Turn off the red LED
    digitalWrite(19, HIGH);
    greenLEDState = HIGH;
    redLEDState = LOW;
    PublishToFirebase(ledRedTopic, redLEDState);
    PublishToFirebase(ledGreenTopic, greenLEDState);
  }
}

void PublishMPU6050()
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  PublishToFirebase(mpu6050AccelXTopic, a.acceleration.x);
  Serial.print("\n\nAcceleration X: ");
  Serial.print(a.acceleration.x);

  PublishToFirebase(mpu6050AccelYTopic, a.acceleration.y);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  if (a.acceleration.y > 2.5)
  {
    tone(3, 1000);
  }
  else
  {
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

// Publish data to Firebase //
void PublishToFirebase(const char *topic, int value)
{
  if (Firebase.ready() && SignUpOK)
  {
    if (Firebase.RTDB.setInt(&fbdo, topic, value))
    {
      Serial.println();
      Serial.print(value);
    }
    else
    {
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  }
}

// Recieve data from firebase
int GetFromFirebase(const char *topic)
{
  if (Firebase.ready() && SignUpOK)
  {
    if (Firebase.RTDB.getInt(&fbdo, topic))
    {
      if (fbdo.dataType() == "int")
      {
        int value;
        value = fbdo.intData();
        Serial.println();
        Serial.print(value);
        return value;
      }
      else
      {
        Serial.println("FAILED (NOT INT): " + fbdo.errorReason());
        return -1;
      }
    }
    else
    {
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  }
  else
  {
    Serial.println("Firebase not ready or SignUp failed");
  }
}

// Firebase Setup
void connectToFirebase()
{
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "33sSFssss3@outlook.com", "MkDpI9m22"))
  {
    Serial.println("Sign Up OK");
    SignUpOK = true;
  }
  else
  {
    Serial.printf("Sign Up failed: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

// Connect to WiFi Module in ESP32
void connectToWiFi()
{
  Serial.println("\nConnecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void MPU6050Config()
{
  Wire.begin(CUSTOM_SDA, CUSTOM_SCL);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  // Try to initialize
  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
    {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.println("Accelerometer range set to: +-8G");

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.println("Gyro range set to: +- 500 deg/s");

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.println("Filter bandwidth set to: 5 Hz");

  Serial.println("");
  delay(100);
}
