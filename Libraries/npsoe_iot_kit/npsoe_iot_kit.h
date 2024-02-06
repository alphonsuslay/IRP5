// Written by Wong SS
// Refer here for the board info
// https://docs.google.com/document/d/1C4AIoC-8wOmh02Mw6mXvVnlVwKopI7r9Dgw8xERL61Y/preview

// Updated 2020-02-11 based on version 1.1 PCB
// Updated 2021-10-13 based on version 2.x PCB
// Updated 2022-10-19

#ifndef _NPSOE_IOT_KIT_H
#define _NPSOE_IOT_KIT_H

#include <Arduino.h>

#define LED_R         23 // LED pin
#define LED_R_ON_VAL   0 // LOW to on
#define LED_R_OFF_VAL  1 // HIGH to off
#define LED_Y         19
#define LED_Y_ON_VAL   0
#define LED_Y_OFF_VAL  1
#define LED_G         18
#define LED_G_ON_VAL   0
#define LED_G_OFF_VAL  1
#define PB1           27 // Push button pins
#define PB2           25
#define PB1_ON_VAL     0 // LOW when pressed
#define PB1_OFF_VAL    1 // HIGH when released
#define PB2_ON_VAL     0
#define PB2_OFF_VAL    1

#define G0_D0         33 // Grove G0 1st digital pin
#define G0_A0         A5 // Grove G0 1st analog pin (same pin as above)

#define G1_D0         15 // Grove G1 1st digital pin
#define G1_D1         25 // Grove G1 2nd digital pin

#define G2_D0         32
#define G2_A0         A4
#define G2_D1         27

#define G3_D0         16 // these are UART pins too
#define G3_D1         17

#define G4_D0         22   // these are I2C pins too
#define G4_D1         21

// Grove uart pins
#define RX2_PIN       16
#define TX2_PIN       17

// Grove i2c pins at
// For esp32, may need to use this: Wire.begin(I2C_SDA,I2C_SCL);
// Can set speed to e.g. 400000: Wire.setClock(400000);
#define I2C_SCL       22
#define I2C_SDA       21

// this pin reserved for SPI Slave Select?
#define SPI_SS             27

#define G_LIGHT_PIN         A3 // Grove kit A6
#define G_SOUND_PIN         A6 // Grove kit A2
#define G_ROTARY_POT_PIN    A0 // Grove kit A0
#define G_LED_PIN           12 // Grove kit D4
#define G_BUZZER_PIN        14 // Grove kit D5
#define G_BUTTON_PIN        26 // Grove kit D6
#define G_TEMP_HUMIDITY_PIN 13 // Grove kit D3 DHT11
#define G_DHT11_PIN         13 // Grove kit D3 DHT11

// Uno footprint pin positions
// UD0 to 2 not available
//      uno         esp32
#define UD3         13
#define UD4         12
#define UD5         14
#define UD6         26
// UD7 not available
#define UD8         25
#define UD9         27
#define UD10        15
#define UD11        23
#define UMOSI       23	// SPI MOSI pin
#define UD12        19
#define UMISO       19	// SPI MISO pin
#define UD13        18
#define USCK        18	// SPI SCK pin
#define USDA        21	// I2C SDA pin
#define USCL        22	// I2C SCL pin

//      uno         esp32
#define UA0         A0 // in only
#define UA1         A3 // in only
#define UA2         A6 // in only
#define UA3         A4
#define UA4         A5
// UA5 not available

typedef uint8_t  U8;    // short names of standard fixed size integer types
typedef uint16_t U16;
typedef uint32_t U32;
typedef int8_t   S8;
typedef int16_t  S16;
typedef int32_t  S32;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef int8_t   sint8;
typedef int16_t  sint16;
typedef int32_t  sint32;
typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;

#endif
